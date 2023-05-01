/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "math.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    int T=44;
    int Amp=127;
    int i=0;
    int value1;
    int value2;
    int value3;
    int dephas1 = 0;
    int dephas2 = 3.1415/3;
    int Delay = 0;
    float freq = 50;

    // Initialiser les valeurs précédentes des potentiomètres à -1

    ADCconv_Start();
    ADCconv_StartConvert();

    PWM_1_Start();
    PWM_2_Start();
    PWM_3_Start();

    for(;;)
    {
        // Récupérer la valeur actuelle des potentiomètres
        int16 RESULTAT_ADC_amp = ADCconv_GetResult16(1);
        float TENSION_V_amp = ADCconv_CountsTo_Volts(RESULTAT_ADC_amp);
        int new_amp = (int) (TENSION_V_amp*62);

        int16 RESULTAT_ADC_freq = ADCconv_GetResult16(0);
        float TENSION_V_freq = ADCconv_CountsTo_Volts(RESULTAT_ADC_freq);
        float new_freq = TENSION_V_freq*25;

        // Vérifier si la nouvelle valeur de l'amplitude est différente de l'ancienne
        if ((new_amp < 0.95*Amp) || (new_amp> 1.05*Amp)){
            Amp = new_amp;
        }

        // Vérifier si la nouvelle valeur de la fréquence est différente de l'ancienne
        if (((new_freq < 0.9*freq) || (new_freq > 1.1*freq))&&((new_freq > 0.65*freq)&&(new_freq < 1.45*freq))){
            freq = new_freq;
            if (freq >= 11.4){
                Delay = (int)(3563-272*freq+9.03*pow(freq,2)-0.145*pow(freq,3)+0.000894*pow(freq,4));
            }
            else if((freq < 11.4)&&(freq>0.4)) {
                Delay = (int)(20881*pow(freq,-1.05));
            }
            else {
                Delay=50000;
            }
   
        }
        for(i=0;i<=T;i++){
            value1 = 127 + Amp*sin(2*3.1415*i/T+dephas1);
            value2 = 127 + Amp*sin(2*3.1415*i/T+dephas2*2);
            value3 = 127 + Amp*sin(2*3.1415*i/T+dephas2*4);
            PWM_1_WriteCompare(value1);
            PWM_2_WriteCompare(value2);
            PWM_3_WriteCompare(value3);
            CyDelayUs(Delay);
        }
    }
}