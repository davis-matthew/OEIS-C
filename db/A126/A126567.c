/*
 * A126567  Sequence generated from the E6 Cartan matrix.
 * https://oeis.org/A126567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the (0,0) entry of the n‑th power of the Cartan matrix of
 *            E6, i.e. a(n) = (M^n)[0][0] where M = [2,-1,0,0,0,0;
 *            -1,2,-1,0,0,0; 0,-1,2,-1,0,-1; 0,0,-1,2,-1,0; 0,0,0,-1,2,0;
 *            0,0,-1,0,0,2]
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base matrix M */
    seq_t M[6][6];
    M[0][0] =  2;  M[0][1] = -1; M[0][2] =  0; M[0][3] =  0; M[0][4] =  0; M[0][5] =  0;
    M[1][0] = -1;  M[1][1] =  2; M[1][2] = -1; M[1][3] =  0; M[1][4] =  0; M[1][5] =  0;
    M[2][0] =  0;  M[2][1] = -1; M[2][2] =  2; M[2][3] = -1; M[2][4] =  0; M[2][5] = -1;
    M[3][0] =  0;  M[3][1] =  0; M[3][2] = -1; M[3][3] =  2; M[3][4] = -1; M[3][5] =  0;
    M[4][0] =  0;  M[4][1] =  0; M[4][2] =  0; M[4][3] = -1; M[4][4] =  2; M[4][5] =  0;
    M[5][0] =  0;  M[5][1] =  0; M[5][2] = -1; M[5][3] =  0; M[5][4] =  0; M[5][5] =  2;

    /* identity matrix */
    seq_t result[6][6];
    seq_t base[6][6];
    seq_t temp[6][6];
    for (idx_t i = 0; i < 6; ++i)
        for (idx_t j = 0; j < 6; ++j)
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* copy M into base */
    for (idx_t i = 0; i < 6; ++i)
        for (idx_t j = 0; j < 6; ++j)
            base[i][j] = M[i][j];

    /* binary exponentiation: compute M^n into result */
    idx_t exp = n;
    while (exp > 0)
    {
        if (exp & 1)
        {
            /* result = result * base */
            for (idx_t i = 0; i < 6; ++i)
                for (idx_t j = 0; j < 6; ++j)
                {
                    seq_t sum = 0;
                    for (idx_t k = 0; k < 6; ++k)
                        sum += result[i][k] * base[k][j];
                    temp[i][j] = sum;
                }
            for (idx_t i = 0; i < 6; ++i)
                for (idx_t j = 0; j < 6; ++j)
                    result[i][j] = temp[i][j];
        }

        /* base = base * base */
        for (idx_t i = 0; i < 6; ++i)
            for (idx_t j = 0; j < 6; ++j)
            {
                seq_t sum = 0;
                for (idx_t k = 0; k < 6; ++k)
                    sum += base[i][k] * base[k][j];
                temp[i][j] = sum;
            }
        for (idx_t i = 0; i < 6; ++i)
            for (idx_t j = 0; j < 6; ++j)
                base[i][j] = temp[i][j];

        exp >>= 1;
    }

    return result[0][0];
}
