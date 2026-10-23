/*
 * A127946  Hankel transform of central coefficients of (1+k*x-3x^2)^n, k
 *          arbitrary integer.
 * https://oeis.org/A127946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2^n * (-3)^((n+1)*n/2)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 7, i128 n <= 11,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* exponent exp = n*(n+1)/2 (computed in idx_t to avoid overflow) */
    idx_t exp = n * (n + 1) / 2;

    /* 3^exp as a seq_t */
    seq_t pow3 = seq_pow((seq_t)3, exp);

    /* apply sign: (-3)^exp = 3^exp if exp even, -3^exp if exp odd */
    if (exp & 1) {
        pow3 = -pow3;
    }

    /* 2^n as a seq_t */
    seq_t pow2 = seq_pow((seq_t)2, n);

    /* return the product */
    return pow2 * pow3;
}
