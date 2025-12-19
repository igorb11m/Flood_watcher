#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

static const char *TAG = "TEST_SLEEP";

#define WAKEUP_PIN GPIO_NUM_5
#define LED_PIN GPIO_NUM_0

void blink_task(int times) {
  for (int i = 0; i < times; i++) {
    gpio_set_level(LED_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(LED_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void app_main(void) {
  gpio_reset_pin(LED_PIN);
  gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

  ESP_LOGI(TAG, "Woke up!");
  blink_task(3);

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_GPIO) {
    ESP_LOGW(TAG, "Wakeup cause GPIO");
  }

  ESP_LOGI(TAG, "Setting up wakeup and going to sleep");

  gpio_set_pull_mode(WAKEUP_PIN, GPIO_PULLUP_ONLY);

  ESP_ERROR_CHECK(esp_deep_sleep_enable_gpio_wakeup(1ULL << WAKEUP_PIN,
                                                    ESP_GPIO_WAKEUP_GPIO_LOW));

  esp_deep_sleep_start();
}