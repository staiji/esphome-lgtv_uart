#include "esphome.h"

#define LGTV_BUFFER_LEN   64

class LGTVUARTComponent : public Component, public UARTDevice, public BinarySensor {
  protected:
	uint8_t uart_buffer[LGTV_BUFFER_LEN]{0};
	int offset = 0;
	
  public:
	LGTVUARTComponent(UARTComponent *parent) : UARTDevice(parent) {}

	void setup() override {
	// nothing to do here
	}
	void loop() override {

		while (available()) {
			char c = read();
			this->uart_buffer[offset++] = c;
			if (offset > LGTV_BUFFER_LEN - 1) {
				offset = 0;
				break;
			}

			if (c == 'x') {
				this->uart_buffer[offset] = '\0';
				ESP_LOGD("LGTVUART", "[%d] %s", offset, this->uart_buffer);
				if (memcmp(this->uart_buffer, "a ", 2) == 0) {
					if (memcmp(this->uart_buffer + 5, "OK01x", 5) == 0)
						publish_state(true);
					else if (memcmp(this->uart_buffer + 5, "OK00x", 5) == 0)
						publish_state(false);
				}
				offset = 0;
				break;
			}
		
		}

	}
};
