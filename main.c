/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  ******************************************************************************
  ******************************************************************************
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private variables ---------------------------------------------------------*/
uint8_t race_started = 0;  // Indica si la carrera ya comenzó

/* Function prototypes -------------------------------------------------------*/
void start_race_sequence(void);
void SystemClock_Config(void);
void MX_GPIO_Init(void);

/* Main function -------------------------------------------------------------*/
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();   // Llamada a la función de configuración de reloj
  MX_GPIO_Init();         // Inicialización de GPIO

  /* Infinite loop */
  while (1)
  {
    // Leer el estado del botón (presionado cuando está LOW)
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET && race_started == 0) {  // A2 -> PA4
      start_race_sequence();  // Inicia la secuencia del semáforo
      race_started = 1;  // Asegurarse de que la secuencia no se repita
    }
  }
}

/* Sequence to start the race (traffic light) */
void start_race_sequence(void) {
  // Encender el LED rojo (PB0)
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);  // A3 -> PB0
  HAL_Delay(1000);

  // Apagar el LED rojo y encender el LED amarillo (PC1)
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);  // A4 -> PC1
  HAL_Delay(1000);

  // Apagar el LED amarillo y encender el LED verde (PC0)
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);  // A5 -> PC0
  HAL_Delay(1000);

  // Apagar el LED verde para terminar la secuencia
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
}

/* GPIO Initialization Function */
void MX_GPIO_Init(void) {
  /* Enable GPIO Clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Configure GPIO pins for LEDs (PB0 -> A3, PC1 -> A4, PC0 -> A5) */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  // LED Rojo (A3 -> PB0)

  GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_0;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);  // LED Amarillo (A4 -> PC1) y Verde (A5 -> PC0)

  /* Configure GPIO pin for Push Button (PA4 -> A2) */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;  // Pull-up resistor to read HIGH when not pressed
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* System Clock Configuration Function */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configura el oscilador principal */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Inicializa los relojes CPU, AHB y APB */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/* Error Handler */
void Error_Handler(void)
{
    while(1) {
        // Quedarse atrapado aquí en caso de error
    }
}


