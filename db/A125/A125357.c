/*
 * A125357  Number of base 20 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Tr(A^n) where A[i][j] = 1 if |i-j| <= 4, else 0. a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    const int N = 20;

    /* Build the adjacency matrix once */
    static seq_t baseA[20][20];
    static int base_init = 0;
    if (!base_init)
    {
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                int d = i > j ? i - j : j - i;
                baseA[i][j] = (d <= 4) ? 1 : 0;
            }
        }
        base_init = 1;
    }

    /* Matrices for exponentiation */
    seq_t M[20][20];
    seq_t R[20][20];
    seq_t T[20][20];

    /* Copy base adjacency into M */
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            M[i][j] = baseA[i][j];

    /* Identity matrix for result */
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
            R[i][j] = (i == j) ? 1 : 0;
    }

    idx_t e = n;
    while (e > 0)
    {
        if (e & 1)
        {
            /* R = R * M */
            for (int i = 0; i < N; ++i)
            {
                for (int j = 0; j < N; ++j)
                {
                    seq_t sum = 0;
                    for (int k = 0; k < N; ++k)
                        sum += R[i][k] * M[k][j];
                    T[i][j] = sum;
                }
            }
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    R[i][j] = T[i][j];
        }

        /* M = M * M */
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                seq_t sum = 0;
                for (int k = 0; k < N; ++k)
                    sum += M[i][k] * M[k][j];
                T[i][j] = sum;
            }
        }
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                M[i][j] = T[i][j];

        e >>= 1;
    }

    seq_t result = 0;
    for (int i = 0; i < N; ++i)
        result += R[i][i];

    return result;
}
