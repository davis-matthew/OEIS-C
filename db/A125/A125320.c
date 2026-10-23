/*
 * A125320  Number of base 10 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   mat_trace
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = trace(M^n) for n >= 1, a(0) = 1
 * where M[i][j] = 1 if abs(i-j) <= 3 else 0.
 * This counts directed circular n‑digit strings over
 * the alphabet {0,…,9} with adjacent digits differing by at most 3.
 */

#include <stddef.h>
#include "../utils/mat_trace.h"

/* multiply two 10×10 matrices: res = a · b */
static void mat_mult(seq_t res[10][10],
                     const seq_t a[10][10],
                     const seq_t b[10][10])
{
    seq_t temp[10][10];
    for (idx_t i = 0; i < 10; ++i) {
        for (idx_t j = 0; j < 10; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < 10; ++k) {
                sum += a[i][k] * b[k][j];
            }
            temp[i][j] = sum;
        }
    }
    /* copy result back to res */
    for (idx_t i = 0; i < 10; ++i)
        for (idx_t j = 0; j < 10; ++j)
            res[i][j] = temp[i][j];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* empty string */

    /* adjacency matrix M */
    static const seq_t M[10][10] = {
        {1,1,1,1,0,0,0,0,0,0},
        {1,1,1,1,1,0,0,0,0,0},
        {1,1,1,1,1,1,0,0,0,0},
        {1,1,1,1,1,1,1,0,0,0},
        {0,1,1,1,1,1,1,1,0,0},
        {0,0,1,1,1,1,1,1,1,0},
        {0,0,0,1,1,1,1,1,1,1},
        {0,0,0,0,1,1,1,1,1,1},
        {0,0,0,0,0,1,1,1,1,1},
        {0,0,0,0,0,0,1,1,1,1}
    };

    /* prepare for binary exponentiation */
    seq_t power[10][10];
    seq_t result[10][10];
    seq_t temp[10][10];

    /* copy base matrix into power */
    for (idx_t i = 0; i < 10; ++i)
        for (idx_t j = 0; j < 10; ++j)
            power[i][j] = M[i][j];

    /* identity matrix as initial result */
    for (idx_t i = 0; i < 10; ++i)
        for (idx_t j = 0; j < 10; ++j)
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1ULL) {
            mat_mult(temp, result, power);
            for (idx_t i = 0; i < 10; ++i)
                for (idx_t j = 0; j < 10; ++j)
                    result[i][j] = temp[i][j];
        }
        mat_mult(temp, power, power);
        for (idx_t i = 0; i < 10; ++i)
            for (idx_t j = 0; j < 10; ++j)
                power[i][j] = temp[i][j];
        exp >>= 1ULL;
    }

    return mat_trace(result);
}
