#include <iostream>
#include<cmath>
using namespace std;
int main()
{
	setlocale(LC_ALL, "ru");
	cout.precision(7);
	double n1=0, n2=0;
	const double pi = 3.141592653589793;
	double lambda_mkm = 1.3;
	//3.1% G₂O₂ + 96.9% SiO₂ - сердечник
	double A1[3] = { 0.7028554, 0.4146307, 0.8974540 };
	double I1[3] =  { 0.0727723, 0.1143085, 9.896161 };
	//SiO2 (с гасящими добавками) - оболочка
	double A2[3] = { 0.696750,0.408218,0.890815 };
	double I2[3] = { 0.069066,0.1156624,9.900559 };
	double n1_sqr =1;
	double n2_sqr = 1;
	double temp_val = 0.0;
	cout << "Расчеты для n1:" << endl;
	for (int i = 0; i <3; i++)
	{
		temp_val= A1[i] * (pow(lambda_mkm, 2) / (pow(lambda_mkm,2) - (pow(I1[i],2))));
		cout << "Результирующий коэффициент для i="<<i+1<<" :"<<temp_val << endl;
		n1_sqr += temp_val;
		temp_val = 0.0;
	}
	temp_val = 0.0;
	cout << "n1 в квадрате=" << n1_sqr << endl << endl;
	n1 = sqrt(n1_sqr);
	cout <<"Показатель преломления сердечника:"<< n1 << endl<< endl;
	cout << "Расчеты для n2:" << endl;
	for (int i = 0; i < 3; i++)
	{
		temp_val = A2[i] * (pow(lambda_mkm, 2) / (pow(lambda_mkm, 2) - (pow(I2[i], 2))));
		cout << "Результирующий коэффициент для i=" << i + 1 << " :" << temp_val << endl;
		n2_sqr += temp_val;
		temp_val = 0.0;
	}
	cout << "n2 в квадрате=" << n2_sqr << endl << endl;
	n2 = sqrt(n2_sqr);
	cout << "Показатель преломления облочки:"<<n2 << endl<< endl;
	//Числовая апертура
	double NA = 0.0;
	double del = 0.0;
	if (pow(n1,2)-pow(n2,2)<0)
	{
		cout << "n1<n2,выберите другой сердечник или оболчку" << endl << endl;
		return 0;
	}
	NA = sqrt(pow(n1, 2) - pow(n2, 2));
	if (NA > 0.2)
	{
		cout << "Выберите другие материалы сердечника и оболочки!" << endl << endl;
		return 0;
	}
	cout << "NA=" << NA << endl << endl;
	del = (pow(n1,2)-pow(n2,2))/ (2 * pow(n1, 2));
	cout << "Относительная разность показателей преломления:"<<del << endl<< endl;
	//РАСЧЕТ НОРМИРОВАННОЙ ЧАСТОТЫ
	double V = 0.0;
	double a = 4.15;
	V = (2 * pi * a * NA) / lambda_mkm;
	if (2.405<V)
	{
		cout << "Выберите другие материалы сердечника и оболочки!" << endl << endl;
		return 0;
	}
	cout << "Нормированная частота:"<<V << endl<< endl;
	//РАСЧЕТ ЗАТУХАНИЯ СВЕТОВОДА
	double a_n = 0.0;
	const double tg_del = 2.4 * 1e-12;//тангенс угла диэлектрических потерь в световоде
	a_n = (8.69 * (pi * n1 * pow(10, 9) * tg_del)) / lambda_mkm;//Затухание в результате поглощения
	cout << "Затухание в результате поглощения(a_n):" << a_n << " [дБ/км]" << endl<< endl;
	double a_p = 0.0;//Затухание на рассеяние
	const double K = 1.38e-23;//коэффициент Больцмана
	const double X = 8.1e-11;//коэффициент сжимаемости
	const double T = 1500;
	a_p = (4.34 * 8.0 * pow(pi, 3) * (pow(n1, 2) - 1) * K *T* X * 1e3)/(3 * pow(lambda_mkm * 1e-6, 4));
	cout << "Затухание на рассеяние:"<<a_p << " [дБ/км]" << endl<< endl;
	double a_macro = 0.0;//Потери на макроизгибы 
	double d = 160.0;//диаметр скрутки [мм]
	double S = 80.0;//шаг скрутки [мм]
	a_macro = ((26 * 1e-3) * a) / (del *d* (1 + pow((S / (pi * d)), 2)));
	cout << "Потери на макроизгибы:"<<a_macro << " [дБ/км]" << endl<< endl;
	double U = 0.0;
	U = (12.97 * a * sqrt(del))/ lambda_mkm;
	cout << "U=" << U << endl;
	double w0 = 0.0;//радиус поля моды
	w0 = a * (0.65 + 1.61 * pow(U, -1.5) + 2.879 * pow(U, -6));
	cout << "Радиус поля моды:" << w0 << " [мкм]" << endl<< endl;
	double a_micro = 0.0;// затухание за счет излучения при микроизгибах
	double k = 15.0;//коэффициент зависящий от длины и амплитуды микроизгибы
	double b = 125;//диамтер оболочки [мкм]
	a_micro = (2 * 1e-4 * k * pow(a, 4) * pow(w0, 6)) / (pow(b, 6) * pow(del, 3) * pow(lambda_mkm, 4));
	cout<<"Дополнительное затухание за счет излучения при микроизгибах:"<<a_micro<< " [дБ/км]" << endl<< endl;
	double A = 0.0;
	A = a_n + a_p + a_macro + a_micro;
	cout << "Полные потери в волоконном световоде составят:" << A << " [дБ/км]" << endl<<endl;
	//РАСЧЕТ ДИСПЕРСИИ ОПТИЧЕСКОГО ВОЛОКНА
	double B = 0.0;//Коэффициент удельной волноводной дисперсии
	const double c = 3 * 1e5;//скорость света [км/с]
	B = ((2.0 * pow(n1, 2) * del ) / (lambda_mkm * c))*1e9;
	cout << "Коэффициент удельной волноводной дисперсии:" << B << " [пс/(км*нм)]" << endl<< endl;
	double der_n_lamb = 0.0;//Производная
	double temp = 0.0;
	double temp_res = 0.0;
	cout << "Для производной:" << endl;
	for (int i = 0; i < 3; i++)
	{
		temp = (A1[i] * pow(I1[i], 2)) / pow(pow(lambda_mkm, 2) - pow(I1[i], 2), 2);
		cout << "Результирующий коэффициент для i=" << i + 1 << " :" << temp << endl;
		temp_res += temp;
		temp = 0.0;
	}
	der_n_lamb = (-lambda_mkm * temp_res) / n1;
	cout << "Производная(delta_n/delta_lambda):" << der_n_lamb << endl << endl;
	double M = 0.0;//Коэффициент удельной материальной дисперсии
	temp = 0.0;//обнуляем временную переменную для повторного использования
	temp_res = 0.0;//обнуляем временную результирующую переменную для повторного использования
	cout << "Для удельной материальной дисперсии:" << endl;
	for (int i = 0; i < 3; i++)
	{
		temp = ((A1[i] * pow(I1[i], 2) * (3 * pow(lambda_mkm, 2) + pow(I1[i], 2)) / pow(pow(lambda_mkm, 2) - pow(I1[i], 2), 3)))-pow(der_n_lamb,2);
		cout << "Результирующий коэффициент для i=" << i + 1 << " :" << temp << endl;
		temp_res += temp;
		temp = 0.0;
	}
	M = (lambda_mkm * temp_res * 1e9) / (c * n1);
	cout << "Коэффициент удельной материальной дисперсии" << M <<" [пс/(км нм)]"<< endl<< endl;
	double t_m = 0.0;//уширение импульса за счет материальной дисперсии
	const double del_lambda = 0.3;//[нм]
	t_m = del_lambda * M;
	cout << "Уширение импульса за счет материальной дисперсии(t_m):" << t_m << " [пс/км]" << endl<< endl;
	double t_v = 0.0;//уширение импульса за счет волновой дисперсии
	t_v = del_lambda * B;
	cout << "Уширение импульса за счет волновой дисперсии(t_v):" << t_v << " [пс/км]" << endl<< endl;
	double t = abs(t_m + t_v);
	cout << "Полное уширение импульса(t):" << t << " [пс/км]" << endl<< endl;
	double del_F = (0.44 * 1e12) / t;
	cout << "Максимальная ширина полосы пропускания на 1 км оптической линии:" << del_F << " [Гц*км]" << endl<< endl;
	//ОПРЕДЕЛЕНИЕ ДЛИНЫ РЕГЕНЕРАЦИОННОГО УЧАСТКА ПО ЗАТУХАНИЮ ОПТИЧЕСКОГО КАБЕЛЯ
	double P_pr_min1 = 0.0;
	const double a_vh = 3.0;//[дБ]
	const double a_pc = 0.3;//[дБ]
	const double a_nc = 0.02;//[дБ]
	const double a_vih = 4.0;//[дБ]
	const double l_cd = 2.0;//[км]
	const double P_per = (10.0 * log10(15.0))-6.0;
	double P_pr_min2 = 0.0;//Вводится пользователем в зависимости от выбранного значения на графике P_per_min(B)
	cout << "Введите значение P_пер_мин по графику:";
	cin >> P_pr_min2;
	const double P = P_per - a_vh - a_vih - P_pr_min2;
	cout << "Энергетический потенциал аппаратуры(P):" << P << " [дБм]" << endl<< endl;
	double l_p = ((P + a_nc - 2.0 * a_pc) * l_cd )/ (a_nc + A * l_cd);
	cout << "Минимальная длина регенрационного участка(l_p_min):" << l_p << " [км]" << endl << endl;
	P_pr_min1 = P_per - a_vh - 2.0 * a_pc - ((l_p / l_cd) - 1.0) * a_nc - A * l_p - a_vih;
	cout << "Минимально допустимая мощность на входе фотоприемника(P_pr_min1):" << P_pr_min1 << " [дБм]" << endl << endl;
	//ОПРЕДЕЛЕНИЕ ДЛИНЫ РЕГЕНЕРАЦИОННОГО УЧАСТКА ПО ПРОПУСКНОЙ СПОСОБНОСТИ ОПТИЧЕСКОГО КАБЕЛЯ
	const double B1 = 622;//Скорость передачи информации [Мбит/с]		
	const double t_s = t * 1e-12;
	const double B2 = 0.25 / t_s;
	cout << "Дисперсионные явления в волоконном световоде приводят к появлению межсимвольной интерференции,"<<endl<<" для уменьшения которой необходимо, чтобы выполнялось следующее условие B(Мбитс / c) <= " << B2 <<endl<< endl;
	if (B1*1e6 > B2)
	{
		cout << "Условие B<=(0.25/t) не выполняется" << endl;
		cout << "Неправильно подобраны значения n1 и n2,вернитесь в начало и подберите другие материалы для сердечника и оболочки" << endl << endl;
		return 0;
	}
	
	const double l_p2 = (0.25 * 1e6) / (B1 * t);
	cout << "Максимаотная длина регенерационного участка(l_p_max):" << l_p2 << " [км]" << endl << endl;
	
	return 0;
}

