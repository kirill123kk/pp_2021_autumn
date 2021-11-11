#include <gtest/gtest.h>
#include <vector>
#include <gtest-mpi-listener.hpp>
#include <mpi.h>
#include "lex_ordering.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    RUN_ALL_TESTS();
    return 0;
}


TEST(MPILexicalOrdering, IdenticalTest) {
    int procRank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        //Generating initial string
        int length = rand() * 4000 / RAND_MAX + 1000; // From 1000 to 5000
        char* string1 = get_random_string(length);
        char* string2 = new char[length + 1];
        memcpy(string2, string1, sizeof(char) * (length + 1));

        int result = omp_lex_compare(string1, string2, length);

        ASSERT_EQ(result, 0);
        delete[] string1;
        delete[] string2;
    }
}

TEST(MPILexicalOrdering, GreaterInconsistencyInStartTest) {
    int procRank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        //Generating initial string
        int length = rand() * 4000 / RAND_MAX + 1000; // From 1000 to 5000
        char* string1 = get_random_string(length);
        char* string2 = new char[length + 1];
        memcpy(string2, string1, sizeof(char) * (length + 1));

        //Adding inconsistency in start
        string1[0] = 'A';
        string2[0] = 'B';

        int result = omp_lex_compare(string1, string2, length);
        ASSERT_EQ(result, 1);
        delete[] string1;
        delete[] string2;
    }

}
TEST(MPILexicalOrdering, LowerInconsistencyInStartTest) {
    int procRank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        //Generating initial string
        int length = rand() * 4000 / RAND_MAX + 1000; // From 1000 to 5000
        char* string1 = get_random_string(length);
        char* string2 = new char[length + 1];
        memcpy(string2, string1, sizeof(char) * (length + 1));

        //Adding inconsistency in start
        string1[0] = 'B';
        string2[0] = 'A';

        int result = omp_lex_compare(string1, string2, length);

        ASSERT_EQ(result, -1);
        delete[] string1;
        delete[] string2;
    }

}

TEST(MPILexicalOrdering, GreaterInconsistencyInEndTest) {
    int procRank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        //Generating initial string
        int length = rand() * 4000 / RAND_MAX + 1000; // From 1000 to 5000
        char* string1 = get_random_string(length);
        char* string2 = new char[length + 1];
        memcpy(string2, string1, sizeof(char) * (length + 1));

        //Adding inconsistency in start
        string1[length - 5] = 'A';
        string2[length - 5] = 'B';

        int result = omp_lex_compare(string1, string2, length);

        ASSERT_EQ(result, 1);
        delete[] string1;
        delete[] string2;
    }
}
TEST(MPILexicalOrdering, LowerInconsistencyInEndTest) {
    int procRank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        //Generating initial string
        int length = rand() * 4000 / RAND_MAX + 1000; // From 1000 to 5000
        char* string1 = get_random_string(length);
        char* string2 = new char[length + 1];
        memcpy(string2, string1, sizeof(char) * (length + 1));

        //Adding inconsistency in start
        string1[length - 5] = 'B';
        string2[length - 5] = 'A';

        int result = omp_lex_compare(string1, string2, length);

        ASSERT_EQ(result, -1);
        delete[] string1;
        delete[] string2;
    }

}
