#include "example_task.h"
#include "pid.h"
#include "motor.h"

PID pid_m3508_speed[13];     
float m3508_speed_error[13];     
float m3508_target_speed[13];    


static void MotorControl_Init(void)
{
    for (int i = 1; i <= 12; i++)
    {
        PID_init(&pid_m3508_speed[i],
                 3.0f,      
                 0.15f,      
                 0.0f,      
                 1200.0f,   
                 15000.0f); 
    }
}


static inline void Motor_SetSpeedTarget(uint8_t can, uint8_t id, float target_rpm)
{
    CanMotor *motor;

    
    if (can == 1)
        motor = &CAN1_Motors[id];
    else
        motor = &CAN2_Motors[id];
    
		m3508_target_speed[id] = target_rpm;
		m3508_speed_error[id] = target_rpm - motor->speed; 
    
    float current_out = PID_calc(&pid_m3508_speed[id], motor->speed, target_rpm);

    if (can == 1)
        CAN1_MotorCurrents[id] = (int16_t)current_out;
    else
        CAN2_MotorCurrents[id] = (int16_t)current_out;
}



void example_task(void* arg)
{
    MotorControl_Init();  

    
    float target_speed = 500.0f;

    while(1)
    {
        
        Motor_SetSpeedTarget(1, 3, target_speed);

        
        CanMotor_SendCurrent(CAN1_MOTOR_1_TO_4);

        delay(1);   
    }
}

