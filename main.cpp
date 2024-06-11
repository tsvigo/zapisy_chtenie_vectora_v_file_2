// #include <QCoreApplication>

// int main(int argc, char *argv[])
// {
//     QCoreApplication a(argc, argv);

//     return a.exec();
// }
//###########################################################################
#include <QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <vector>
//###########################################################################
// Объяснение
//     Заголовки и пространства имен:

//                                     QCoreApplication для консольного приложения.
//                                     QFile для обработки файлов.
//                                     QDataStream для сериализации двоичных данных.
//                                     vector для std::vector контейнера.
//                                     algorithm для сравнения.
//###########################################################################
// Function to write a vector to a file
// writeVectorToFile:

//                     Открывает файл в режиме только для записи.
//                     Использует QDataStream для сериализации размера и элементов вектора.
//                     Обрабатывает ошибки, если файл не может быть открыт.
void writeVectorToFile(const std::vector<unsigned long long>& vec, const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qCritical() << "Could not open file for writing:" << filename;
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_0); // Ensure the same version for reading and writing
    out << static_cast<quint32>(vec.size()); // Write the size of the vector
    for (unsigned long long value : vec) {
        out << static_cast<quint64>(value); // Write each element of the vector
    }

    file.close();
}
//###########################################################################
// Function to read a vector from a file
// readVectorFromFile:

//                      Открывает файл в режиме только для чтения.
//                      Использует QDataStream для десериализации размера и элементов вектора.
//                      Обрабатывает ошибки, если файл не может быть открыт.

std::vector<unsigned long long> readVectorFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Could not open file for reading:" << filename;
        return {};
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_0); // Ensure the same version for reading and writing
    quint32 size;
    in >> size; // Read the size of the vector

    std::vector<unsigned long long> vec(size);
    for (quint32 i = 0; i < size; ++i) {
        quint64 value;
        in >> value; // Read each element of the vector
        vec[i] = static_cast<unsigned long long>(value);
    }

    file.close();
    return vec;
}
//###########################################################################
// Function to compare two vectors
// Сравнительные векторы:

//                         Сравнивает два вектора, используя оператор равенства (==).
bool compareVectors(const std::vector<unsigned long long>& vec1, const std::vector<unsigned long long>& vec2) {
    return vec1 == vec2;
}
//###########################################################################
// Function to print the contents of a vector
void printVector(const std::vector<unsigned long long>& vec) {
    for (unsigned long long value : vec) {
        qDebug() << value;
    }
}


//###########################################################################
// Главная:

//           Создает пример вектора unsigned long long значений.
//           Записывает его в файл.
//           Считывает его обратно из файла.
//           Проверяет, идентичны ли исходный и прочитанный векторы.
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Example vector
    std::vector<unsigned long long> originalVector = {12345678901234567ULL, 23456789012345678ULL, 34567890123456789ULL};
    QString filename = "/home/viktor/my_projects_qt_2/zapisy_chtenie_vectora_v_file_2/vector.dat";

    // Write vector to file
    writeVectorToFile(originalVector, filename);

    // Read vector from file
    std::vector<unsigned long long> readVector = readVectorFromFile(filename);
//###########################################################################
    // Print read vector
    qDebug() << "Read vector:";
    printVector(readVector);
//###########################################################################
    // Verify correctness
    if (compareVectors(originalVector, readVector)) {
        qDebug() << "The vector was written and read correctly.";
    } else {
        qDebug() << "The vector was NOT written and read correctly.";
    }

    return a.exec();
}
