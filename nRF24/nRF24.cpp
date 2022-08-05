#pragma region includes
#include "nRF24.h"
#pragma endregion

nRF24::nRF24(){
}
// Alıcıyı hazılar.
RF24 nRF24::nRF24AliciKurulum(RF24 radyo, unsigned int gucSeviyesi, rf24_datarate_e veriFrekansi){
    // Radyoyu başlatır.
    radyo.begin();
    // Güç seviyesini parametreye göre ayarlar.
    radyo.setPALevel(gucSeviyesi);
    // Veri gönderme frekansını ayarlar.
    radyo.setDataRate(veriFrekansi);
    // Karışmayacak bir kanal ayarlanır. 
    /**
     * @note 2000mHZ + param.
     */
    radyo.setChannel(124);
    // Bir adres ile yazma kanalı, diğer adresle ise okuma kanalı açar. 
    radyo.openWritingPipe(adres[0]);
    radyo.openReadingPipe(1, adres[1]);
    // Veri için dinlemeye başlar.
    radyo.startListening();
    radyoCalisti = true;
    return radyo;
}
// Göndericiyi hazırlar.
RF24 nRF24::nRF24VericiKurulum(RF24 radyo, unsigned int gucSeviyesi, rf24_datarate_e veriFrekansi){
    // Radyoyu başlatır.
    radyo.begin()
    // Güç seviyesini parametreye göre ayarlar.
    radyo.setPALevel(gucSeviyesi);
    // Veri gönderme frekansını ayarlar.
    radyo.setDataRate(veriFrekansi);
    // Karışmayacak bir kanal ayarlanır. 
    /**
     * @note 2000mHZ + param.
     */
    radyo.setChannel(124);
    // Bir adres ile yazma kanalı, diğer adresle ise okuma kanalı açar.
    radyo.openWritingPipe(adres[1]);
    radyo.openReadingPipe(1, adres[0]);
    radyoCalisti = true;
    return radyo;
}
// Alıcıya paketleri gönderir ve sonrasında alıcının gönderdiği paketleri alır.
void nRF24::nRF24VeriGonder(RF24 radyo, int gonderilecekVeri[], const int gonderilecekElementSayisi){
    // NOT: sizeof() kullanarak gonderilecek olan veri dizisinin uzunluğunu al.
    unsigned int veri[gonderilecekElementSayisi];
    for(int i = 0; i < gonderilecekElementSayisi; i++){
        veri[i] = gonderilecekVeri[i];
    }
    // Dinlemediğinden emin olur.
    radyo.stopListening();
    // Veri başarıyla gönderildi mi?
    radyo.write(&veri, sizeof(veri));

    // Alıcıdan gelecek olan paketler için dinlemeye başlar.
    radyo.startListening();
    unsigned long baslangic = millis();
      // 200 ms boyunca dinlemeye devam eder.
    while ( ! radyo.available() ) {
        // Eğer süre içerisinde veri gelmezse geri döner.
        if (millis() - baslangic > 100) {
            return;
        }
    }

    // Bufferdaki veriyi okur.
    unsigned int veriRx[gonderilecekElementSayisi];
    radyo.read( &veriRx, sizeof(veriRx) );

    // Debug amaçlı alınan gönderilen veriyi ve alınan veriyi monitöre yazar.
    // Bir süre sonra tekrar dener.
    //delay(10);
}
/**
 * @brief Göndericiden paketleri alır. 
 * 
 * @param radyo 
 * @param gonderilecekElementSayisi 
 */
void nRF24::nRF24VeriAl(RF24 radyo, int (&yazilacakArr)[4], const int gonderilecekElementSayisi){     
    // Göndericiden alacağımız verilerin yazıldığı array.
    unsigned int veri[gonderilecekElementSayisi];

    // Alınabilecek veri var mı?
    if (radyo.available()) {
        // Veri olduğu sürece veriyi okumaya devam eder.
        while (radyo.available()) {
          radyo.read(&veri, sizeof(veri));
        }

        // Dinlemeyi durdur ve geri gödermek istediğimiz verileri gönderir.
        radyo.stopListening();
        radyo.write(&veri, sizeof(veri));

        // Paket kaçırmamak için dinlemeye devam eder.
        radyo.startListening();
        
        // Alınabilmesi için veriyi değişkenlere yazar.
        for(int i = 0; i < gonderilecekElementSayisi; i++){
            yazilacakArr[i] = veri[i];
        }
    }
}