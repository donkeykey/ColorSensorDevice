#include <Arduino.h>

void setup();
void loop();
int shiftIn12(int dataPin, int clockPin);
#line 1 "src/sketch.ino"
// Arduinoのピン定義
const int doutPin = 2;   // Dout
const int rangePin = 4;  // Range
const int ckPin = 5;     // CK
const int gatePin = 6;   // Gate

const int redPin = 9;   // Gate
const int greenPin = 10;   // Gate
const int bluePin = 11;   // Gate

void setup() {
	// put your setup code here, to run once:

	//Serial.begin(9600);

	// ピンモードを設定する。doutPinは入力，それ以外は出力。
	pinMode(doutPin, INPUT);
	pinMode(rangePin, OUTPUT);
	pinMode(ckPin, OUTPUT);
	pinMode(gatePin, OUTPUT);

	pinMode(redPin, OUTPUT);
	pinMode(greenPin, OUTPUT);
	pinMode(bluePin, OUTPUT);

	analogWrite(redPin,0);
	analogWrite(greenPin,0);
	analogWrite(bluePin,0);
}

void loop() {
	// put your main code here, to run repeatedly: 
	int red, green, blue ,sum;  // 測定した値を格納するための変数
	int integration = 50;  // 測定時間(ミリ秒)を格納する変数
	char s[64];            // シリアルコンソールに出力する文字列を保持する変数

	digitalWrite(gatePin, LOW);        // GateとCKをLOWにする。
	digitalWrite(ckPin, LOW);
	digitalWrite(rangePin, HIGH);      // RangeをHIGHにする。

	digitalWrite(gatePin, HIGH);       // GateをHIGHにして測定開始。
	delay(integration);                // 測定時間だけ待つ。
	digitalWrite(gatePin, LOW);        // GateをLOWにして測定終了。
	delayMicroseconds(4);              // 4ミリ秒待つ。
	red = shiftIn12(doutPin, ckPin);   // 赤色の値を読む。
	red = red/1.4;					//補正
	delayMicroseconds(3);              // 3ミリ秒待つ。
	green = shiftIn12(doutPin, ckPin); // 緑色の値を読む。
	green = green/1.1;					//補正
	delayMicroseconds(3);              // 3ミリ秒待つ。
	blue = shiftIn12(doutPin, ckPin);  // 青色の値を読む。
	blue = blue/1.1;					//補正

	sprintf(s, "Red = %4d, Green = %4d, Blue = %4d", red, green, blue);
	//Serial.println(s);

	red = map(red,0,4095,0,255);
	green = map(green,0,4095,0,255);
	blue = map(blue,0,4095,0,255);
	sum = red + green + blue; 
	if(sum < 4){
		red = 0;
		green = 0;
		blue = 0;
	}else if(sum < 255){
		red = (255/sum) * red;
		green = (255/sum) * green;
		blue = (255/sum) * blue;
	}
	analogWrite(redPin,255 - red);
	analogWrite(greenPin,255 - green);
	analogWrite(bluePin,255 - blue);

	//delay(2000);
}

//12ビットの値を読み込む関数(LSBから送信されるデータを想定)
int shiftIn12(int dataPin, int clockPin) {
	int value = 0;

	for (int i = 0; i < 12; i++) {
		digitalWrite(clockPin, HIGH);           // クロックをHIGHにする
		value |= digitalRead(dataPin) << i;     // データピンの値を読み取り，所定のビットを設定する。
		digitalWrite(clockPin, LOW);            // クロックピンをLOWにする。
	}

	return value;
}
