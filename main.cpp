
// Infrared LED: https://akizukidenshi.com/catalog/g/gI-03261/
// Infrared Receiver Module: https://akizukidenshi.com/catalog/g/gI-04659/


#include "mbed.h"


Serial pc(USBTX, USBRX);
DigitalOut led(D12);
InterruptIn push_button(A0);
Timer timer;


static const int on[34] = {0x01, 0x10, 0x00, 0x40, 0xbf, 0xff, 0x00, 0xcc, 0x33, 0x92, 0x6d, 0x13, 0xec, 0x6c, 0x93, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x33, 0xcc, 0xd1, 0x2e, 0x00, 0xff, 0x00, 0xff, 0x00};
static const int off[34] = {0x01, 0x10, 0x00, 0x40, 0xbf, 0xff, 0x00, 0xcc, 0x33, 0x92, 0x6d, 0x13, 0xec, 0x6c, 0x93, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x33, 0xcc, 0xc1, 0x3e, 0x00, 0xff, 0x00, 0xff, 0x00};
static const int T = 425;


void led_blink(const int t = T) {
    timer.start();
    while(timer.read_us() <= t) {
        led = 1;
        wait_us(8.8);
        led = 0;
        wait_us(17.5);
    }
    led = 0;
    timer.stop();
    timer.reset();
}

void leader() {
    led_blink(8*T);
    wait_us(4*T);
}

void pulse(const int off_length) {
    led_blink();
    wait_us(T * off_length);
}

void turn(const int *signal) {
    leader();
    for(int i = 0; i < 34; ++i) {
        for(int j = 0; j < 8; ++j) {
            if (signal[i] & (1 << j)) {
                // 1 |_|---
                pulse(3);
            } else {
                // 0 |_|-
                pulse(1);
            }
        }
    }
}

void turn_on() { turn(on); }
void turn_off() { turn(off); }

int main() {
    pc.baud(115200);
    push_button.fall(&turn_on);
    push_button.rise(&turn_off);
    push_button.mode(PullUp);
}