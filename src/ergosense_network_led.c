#include <FreeRTOS.h>
#include <task.h>
#include "ergosense_network_led.h"
#include "mgos.h"
#include "mgos_wifi.h"
#include "mgos_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static void _ergosense_led_task(void *params)
{
  int led = (int) params;

  mgos_gpio_set_mode(led, MGOS_GPIO_MODE_OUTPUT);

  while (1)
  {
    enum mgos_wifi_status status = mgos_wifi_get_status();

    switch (status)
    {
      case MGOS_WIFI_CONNECTED:
      case MGOS_WIFI_IP_ACQUIRED:
        mgos_gpio_write(led, true);
        break;
      default:
        mgos_gpio_toggle(led);
    }

    vTaskDelay(ERGOSENSE_NETWORK_LED_BLINK_INTERVAL / portTICK_RATE_MS);
  }

  vTaskDelete(NULL);
}

void ergosense_network_led_init(int led)
{
  LOG(LL_DEBUG, ("Initialize network monitor LED on PIN %d", led));

  xTaskCreate(&_ergosense_led_task, "_ergosense_led_task", configMINIMAL_STACK_SIZE, (void *) led, 1, NULL);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
