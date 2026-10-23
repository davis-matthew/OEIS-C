/*
 * A125423  Number of base 11 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A[i][j] = 1 if |i-j| <= 7, 0 otherwise.
 *            The sequence counts base‑11 circular n‑digit numbers with
 *            adjacent digits differing by at most 7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static const seq_t ADJ[11][11] = {
    {1,1,1,1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,1,0,0},
    {1,1,1,1,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1,1,1,1},
    {0,0,1,1,1,1,1,1,1,1,1},
    {0,0,0,1,1,1,1,1,1,1,1}
};

static void mat_mult(const seq_t A[11][11], const seq_t B[11][11], seq_t C[11][11])
{
    seq_t temp;
    for (idx_t i = 0; i < 11; ++i) {
        for (idx_t j = 0; j < 11; ++j) {
            temp = 0;
            for (idx_t k = 0; k < 11; ++k) {
                temp = temp + A[i][k] * B[k][j];
            }
            C[i][j] = temp;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* result = identity matrix */
    seq_t result[11][11];
    seq_t base[11][11];
    seq_t temp[11][11];

    for (idx_t i = 0; i < 11; ++i)
        for (idx_t j = 0; j < 11; ++j)
            result[i][j] = (i == j) ? (seq_t)1 : 0;

    for (idx_t i = 0; i < 11; ++i)
        for (idx_t j = 0; j < 11; ++j)
            base[i][j] = ADJ[i][j];

    /* fast exponentiation with safe temporary buffer */
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mult(result, base, temp);
            for (idx_t i = 0; i < 11; ++i)
                for (idx_t j = 0; j < 11; ++j)
                    result[i][j] = temp[i][j];
        }
        mat_mult(base, base, temp);
        for (idx_t i = 0; i < 11; ++i)
            for (idx_t j = 0; j < 11; ++j)
                base[i][j] = temp[i][j];
        exp >>= 1;
    }

    /* trace of result matrix */
    seq_t trace = 0;
    for (idx_t i = 0; i < 11; ++i)
        trace = trace + result[i][i];

    return trace;
}
