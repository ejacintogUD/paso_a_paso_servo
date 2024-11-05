/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

// Blinking rate in milliseconds
#define BLINKING_RATE     500ms
#define TIEMPO_PASO       10ms
#define TIEMPO_SERVO      200ms  

UnbufferedSerial pc(USBTX, USBRX);

//pines 
PwmOut pwm_servo (D9); // pwm1/1
BusOut pines_servo (D2,D3,D4,D5); // verificar pines 
DigitalIn bnt (BUTTON1);

//hilos 
Thread hilo_paso;
Thread hilo_servo;

// protipos de la funcion 
void paso_a_paso (void);
void servomotor (void);

//constantes 
const char tabla_paso[4] = {0b1100, 0b0110, 0b0011, 0b1001};

//variables 
float angulo;
//

int main()
{
    hilo_servo.start(servomotor);
    hilo_paso.start(paso_a_paso);
    pwm_servo.period_ms(20);
    angulo = 0;
    pwm_servo.period_us(400);
    while (true) {
        ThisThread::sleep_for(BLINKING_RATE);
    }
}


void servomotor (void)
{
    const int ang_minimo = 1000;
    while(true)
    {
        angulo += 100;
        if (angulo > 2000) angulo = 0;    
        pwm_servo.pulsewidth_us(ang_minimo + angulo); // angulo 0-1000 us
        ThisThread::sleep_for(TIEMPO_SERVO);

    }

}

void paso_a_paso (void)
{
    static int i; 
    while(true)
    {
        if (!bnt)  // manecillas del reloj
            {    
            i++;
            if (i>4) i=0;
            }
        else        // antemanecillas del reloj 
            {
            i--;
            if (i<0) i=3;
            }
        pines_servo = tabla_paso[i];
        ThisThread::sleep_for(TIEMPO_PASO);
    }

}