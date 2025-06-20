// 單色 LED PWM 控制，根據模式執行三種燈光節奏
const int ledPin = 9;
String mode = "";
int stress = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    int comma = input.indexOf(',');
    if (comma > 0) {
      mode = input.substring(0, comma);
      mode.trim(); // 去除多餘空白與換行
      stress = input.substring(comma + 1).toInt();
      Serial.print("mode: "); Serial.println(mode); // debug
      Serial.print("stress: "); Serial.println(stress); // debug
    }
  }

  if (mode == "呼吸") {
    // 12秒循環：2秒微光(30)→4秒漸亮(30~200,sin)→4秒漸暗(200~30,sin)→2秒微光(30)
    // 2秒微光
    for (int t = 0; t < 2000; t += 20) {
      analogWrite(ledPin, 30);
      delay(20);
    }
    // 4秒漸亮
    for (int t = 0; t < 4000; t += 20) {
      float theta = (float)t / 4000.0 * PI / 2.0; // 0~PI/2
      int val = 30 + (int)(170.0 * sin(theta)); // 30~200
      analogWrite(ledPin, val);
      delay(20);
    }
    // 4秒漸暗
    for (int t = 0; t < 4000; t += 20) {
      float theta = (float)t / 4000.0 * PI / 2.0; // 0~PI/2
      int val = 200 - (int)(170.0 * sin(theta)); // 200~30
      analogWrite(ledPin, val);
      delay(20);
    }
    // 2秒緩衝微光
    for (int t = 0; t < 2000; t += 20) {
      analogWrite(ledPin, 30);
      delay(20);
    }
  } else if (mode == "中速閃爍") {
    // 1.2秒循環：0.5秒亮(80~255,sin)→0.5秒暗(20)→0.2秒緩衝(20)
    // 0.5秒亮(波形)
    for (int t = 0; t < 500; t += 10) {
      float theta = (float)t / 500.0 * PI; // 0~PI
      int val = 80 + (int)(175.0 * sin(theta)); // 80~255
      analogWrite(ledPin, val);
      delay(10);
    }
    // 0.5秒暗
    for (int t = 0; t < 500; t += 20) {
      analogWrite(ledPin, 20);
      delay(20);
    }
    // 0.2秒緩衝
    for (int t = 0; t < 200; t += 20) {
      analogWrite(ledPin, 20);
      delay(20);
    }
  } else if (mode == "急閃+停頓") {
    // 6秒循環：爆閃4次(0.15亮/0.15滅)→黑1s→長亮0.5s→漸暗0.5s(255~50)→緩衝2s(50~10,sin)
    // 爆閃4次
    for (int i = 0; i < 4; i++) {
      analogWrite(ledPin, 255);
      delay(150);
      analogWrite(ledPin, 0);
      delay(150);
    }
    // 黑屏1秒
    for (int t = 0; t < 1000; t += 20) {
      analogWrite(ledPin, 10); // 背景紅光(10)
      delay(20);
    }
    // 長亮0.5秒
    for (int t = 0; t < 500; t += 10) {
      analogWrite(ledPin, 255);
      delay(10);
    }
    // 快速漸暗0.5秒(255~50)
    for (int t = 0; t < 500; t += 10) {
      int val = 255 - (int)(205.0 * ((float)t / 500.0)); // 線性降
      analogWrite(ledPin, val);
      delay(10);
    }
    // 緩衝2秒(50~10,sin)
    for (int t = 0; t < 2000; t += 20) {
      float theta = (float)t / 2000.0 * PI / 2.0; // 0~PI/2
      int val = 50 - (int)(40.0 * sin(theta)); // 50~10
      if (val < 10) val = 10;
      analogWrite(ledPin, val);
      delay(20);
    }
  } else {
    // 預設背景微光(10)
    analogWrite(ledPin, 10);
    delay(100);
  }
}