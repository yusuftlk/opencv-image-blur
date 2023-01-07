#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include<thread>
#include<vector>

using namespace cv;

int* mavi = (int*)malloc(sizeof(int));
int* yesil = (int*)malloc(sizeof(int));
int* kirmizi = (int*)malloc(sizeof(int));
int sayac = 0, sayac2 = 0;
std::mutex m;

const int komsulukDegeri = 1;
const String dosya_yolu = "C:/image.jpg";
const cv::Mat yuklenenResim = cv::imread(dosya_yolu);


inline void toplamFonk(const int komsuBasX, const int komsuBasY)
{
	*mavi += yuklenenResim.at<cv::Vec3b>(cv::Point(komsuBasX, komsuBasY))[0];
	*kirmizi += yuklenenResim.at<cv::Vec3b>(cv::Point(komsuBasX, komsuBasY))[2];
	*yesil += yuklenenResim.at<cv::Vec3b>(cv::Point(komsuBasX, komsuBasY))[1];
}

inline void ortalamaFonk() {
	*kirmizi = (*kirmizi / sayac);
	*mavi = (*mavi / sayac);
	*yesil = (*yesil / sayac);
}

cv::Mat blurFonksiyonu() {
	cv::Mat islemResim = cv::Mat::zeros(cv::Size(yuklenenResim.cols, yuklenenResim.rows), CV_8UC3);
	std::vector<std::thread> threadler;
	for (int x = 0; x < yuklenenResim.cols; x++)
	{

		for (int y = 0; y < yuklenenResim.rows; y++)
		{
			//komþuluk tarama iþlemi döngüleri
			for (int komsuBasX = x - komsulukDegeri; komsuBasX < x + komsulukDegeri; komsuBasX++)
			{
				for (int komsuBasY = y - komsulukDegeri; komsuBasY < y + komsulukDegeri; komsuBasY++)
				{
					if (komsuBasX >= 0 && komsuBasY >= 0 && komsuBasX < yuklenenResim.cols && komsuBasY < yuklenenResim.rows)
					{

						/*threadler.push_back(std::thread(toplamFonk,komsuBasX,komsuBasY));
						threadler[sayac2].join();
						sayac2++;*/


						toplamFonk(komsuBasX, komsuBasY);
						sayac++;

					}
				}
			}
			ortalamaFonk();

			islemResim.at<cv::Vec3b>(cv::Point(x, y))[0] = *mavi;
			islemResim.at<cv::Vec3b>(cv::Point(x, y))[1] = *yesil;
			islemResim.at<cv::Vec3b>(cv::Point(x, y))[2] = *kirmizi;

			*mavi = 0;
			*yesil = 0;
			*kirmizi = 0;
			sayac = 0;
		}
		//std::thread t1(asd);
		//t1.join();
	}
	return islemResim;
}
int main()
{
	cv::Mat blurResim = blurFonksiyonu();
	cv::imwrite("C:/image.jpg", blurResim);

	/*std::vector<std::thread> threadler;
	for (int i = 0; i < 100; i++)
	{
		threadler.push_back(std::thread(ortalamaFonkKirmizi));
	}
	for (int i = 0; i < 100; i++)
	{
		threadler[i].join();
	}*/
}