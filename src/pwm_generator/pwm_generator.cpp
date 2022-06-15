/**
 * @file pwm_generator.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes PWM Generator class related declerations.
 * @version 0.1
 * @date 2022-06-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "pwm_generator/pwm_generator.h"

PwmOut *PwmGenerator::led_out;
PwmOut *PwmGenerator::pwm_out;
SerialOutput *PwmGenerator::serial;

PwmGenerator::PwmGenerator()
{
    led_out = NULL;
    pwm_out = NULL;
    serial = NULL;
}

PwmGenerator::~PwmGenerator()
{
    delete led_out;
    delete pwm_out;
    serial = NULL;
}

void PwmGenerator::set_pins(PinName led_pin, PinName output_pin)
{
    led_out = new PwmOut(led_pin);
    pwm_out = new PwmOut(output_pin);
}

void PwmGenerator::set_serial_output(SerialOutput *serial_p)
{
    serial = serial_p;
}

void PwmGenerator::start_thread()
{
    if (led_out == NULL || pwm_out == NULL)
    {
        serial->write("  ## Null PWM output object error.");
        while (true)
        {
        }
    }
    else if (serial == NULL)
    {
        serial->write("  ## Null serial output object error.");
        while (true)
        {
        }
    }
    else
    {
        thread.start(PwmGenerator::pwm_generator_thread);
    }
}

void PwmGenerator::pwm_generator_thread()
{
    PWM_PERIOD_DIR pwm_dir = RISING_DIR;
    int pwm_period_ms = 0;

    while (true)
    {
        if (pwm_dir == RISING_DIR)
        {
            if (pwm_period_ms < 100)
            {
                pwm_period_ms++;
            }
            else
            {
                pwm_dir = FALLING_DIR;
            }
        }
        else if (pwm_dir == FALLING_DIR)
        {
            if (pwm_period_ms > 1)
            {
                pwm_period_ms--;
            }
            else
            {
                pwm_dir = RISING_DIR;
            }
        }
        else
        {
            serial->write("  ## Led direction error.");
            while (true)
            {
            }
        }

        led_out->period_ms(pwm_period_ms);
        led_out->write(0.50f);
        pwm_out->period_ms(pwm_period_ms);
        pwm_out->write(0.50f);
        ThisThread::sleep_for(pwm_period_ms);
    }
}