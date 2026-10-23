/*
 * A038225  Top line of 4-wave sequence A038197, also bisection of A006357.
 * https://oeis.org/A038225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min(v(4)*M(4)^n) where v(4) = (1,1,1,1) and
 *            M(i,j)=min(i+1,j+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiply two 4x4 matrices: C = A * B */
static void mat_mul(const seq_t A[4][4], const seq_t B[4][4], seq_t C[4][4]) {
    seq_t temp[4][4];
    for (idx_t i = 0; i < 4; ++i) {
        for (idx_t j = 0; j < 4; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < 4; ++k) {
                sum += A[i][k] * B[k][j];
            }
            temp[i][j] = sum;
        }
    }
    /* copy to C */
    for (idx_t i = 0; i < 4; ++i)
        for (idx_t j = 0; j < 4; ++j)
            C[i][j] = temp[i][j];
}

/* compute a(n) */
seq_t a(idx_t n) {
    /* base matrix M */
    static const seq_t M[4][4] = {
        {1, 1, 1, 1},
        {1, 2, 2, 2},
        {1, 2, 3, 3},
        {1, 2, 3, 4}
    };

    /* identity matrix */
    seq_t result[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    seq_t base[4][4];
    /* copy M into base */
    for (idx_t i = 0; i < 4; ++i)
        for (idx_t j = 0; j < 4; ++j)
            base[i][j] = M[i][j];

    idx_t e = n;
    while (e > 0) {
        if (e & 1LL) {
            mat_mul(result, base, result);
        }
        e >>= 1;
        if (e)
            mat_mul(base, base, base);
    }

    /* compute column sums of result */
    seq_t sums[4] = {0, 0, 0, 0};
    for (idx_t i = 0; i < 4; ++i)
        for (idx_t j = 0; j < 4; ++j)
            sums[j] += result[i][j];

    /* find minimum sum */
    seq_t min_val = sums[0];
    for (idx_t j = 1; j < 4; ++j)
        if (sums[j] < min_val)
            min_val = sums[j];

    return min_val;
}
