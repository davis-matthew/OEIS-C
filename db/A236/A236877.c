/*
 * A236877  Number of (n+1)X(1+1) 0..2 arrays with the maximum plus the upper
 *          median plus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A236877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{C=0..6} 1ᵀ · (T_C)^n · 1 where T_C[i][j] = 1 if the
 *            sum of the three largest values in the 2×2 block formed by
 *            consecutive rows encoded by i and j equals C, otherwise 0. This
 *            counts all (n+1)×2 arrays over {0,1,2} whose maximum + upper
 *            median + lower median of every 2×2 subblock is the...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   sum_top_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sum_top_three.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1 */

    /* adjacency matrices for each possible C, computed once */
    static seq_t T[7][9][9];
    static int initialized = 0;
    if (!initialized) {
        for (int C = 0; C <= 6; ++C)
            for (int i = 0; i < 9; ++i)
                for (int j = 0; j < 9; ++j)
                    T[C][i][j] = (sum_top_three((seq_t)i, (seq_t)j) == (seq_t)C) ? (seq_t)1 : (seq_t)0;
        initialized = 1;
    }

    seq_t total = 0;

    for (int C = 0; C <= 6; ++C) {
        /* start with all possible first rows */
        seq_t res[9];
        for (int i = 0; i < 9; ++i) res[i] = (seq_t)1;

        /* copy the adjacency matrix for this C */
        seq_t mat[9][9];
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                mat[i][j] = T[C][i][j];

        idx_t exp = n;            /* number of 2×2 blocks = number of transitions */

        /* binary exponentiation of the matrix acting on res */
        while (exp) {
            if (exp & (idx_t)1) {
                seq_t newres[9] = {0};
                for (int k = 0; k < 9; ++k) {
                    seq_t sum = 0;
                    for (int j = 0; j < 9; ++j)
                        sum += res[j] * mat[j][k];
                    newres[k] = sum;
                }
                for (int i = 0; i < 9; ++i) res[i] = newres[i];
            }
            /* square the matrix */
            seq_t newmat[9][9] = {0};
            for (int i1 = 0; i1 < 9; ++i1)
                for (int k1 = 0; k1 < 9; ++k1) {
                    seq_t sum = 0;
                    for (int j = 0; j < 9; ++j)
                        sum += mat[i1][j] * mat[j][k1];
                    newmat[i1][k1] = sum;
                }
            for (int i1 = 0; i1 < 9; ++i1)
                for (int j = 0; j < 9; ++j)
                    mat[i1][j] = newmat[i1][j];
            exp >>= 1;
        }

        /* sum all entries of res: number of valid arrays for this C */
        seq_t subtotal = 0;
        for (int i = 0; i < 9; ++i) subtotal += res[i];
        total += subtotal;
    }

    return total;
}
