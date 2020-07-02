
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "DHT11.h"
#include "oled.h"
#include "bmp.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
char RxBuffer1[1];


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);


int main(void)
{

	extern float tem,hum,tem1,hum1;

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();


  MX_NVIC_Init();

	OLED_RESET();
	OLED_INIT();
	OLED_Display_On();
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);//先将指示灯关掉
		
//发送AT指令，发送成功后指示灯亮五秒
	char mode[]="AT+CWMODE=<2>";
	HAL_UART_Transmit_IT(&huart1 ,(uint8_t*)mode,sizeof(mode));//串口发送数据
	char rst[]="AT+RST";
	HAL_UART_Transmit_IT(&huart1 ,(uint8_t*)rst,sizeof(rst));//esp8266重启
	char charameter[]="AT+CWSAP=<TEST>,<000000>,<1>,<0>";
	HAL_UART_Transmit_IT(&huart1 ,(uint8_t*)charameter,sizeof(charameter));//设置参数
	delay_ms(5000);
	OLED_Clear();//清屏
		
	OLED_ShowChinese(12,0,0);//显示“温”
	OLED_ShowChinese(29,0,1);//显示“湿“
	OLED_ShowChinese(46,0,2);//显示“度”
	OLED_ShowChinese(63,0,3);//显示“检”
	OLED_ShowChinese(80,0,4);//显示“测”
	OLED_ShowChinese(97,0,5);//显示“仪”
	
	OLED_ShowChinese(0,3,0);//显示“温”
	OLED_ShowChinese(20,3,2);//显示“度”
	OLED_ShowMH(40,3);//显示“：”
	
	OLED_ShowChinese(0,6,1);//显示“湿“
	OLED_ShowChinese(20,6,2);//显示“度”
	OLED_ShowMH(40,6);//显示“：”
	
  while (1)
  {

		delay_ms(1000);
		DHT11_receive();
		
		OLED_ShowNum(45,3,tem,2,16);//温度
		OLED_ShowNum(45,6,hum,2,16);//湿度
		OLED_ShowDH(65,3);
		OLED_ShowDH(65,6);
		OLED_ShowNum(68,3,tem1,2,16);//温度
		OLED_ShowNum(68,6,hum1,2,16);//湿度

  }
 
}


void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


static void MX_NVIC_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}

void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


