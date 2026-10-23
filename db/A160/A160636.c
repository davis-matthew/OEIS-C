/*
 * A160636  Hankel transform of A114464.
 * https://oeis.org/A160636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 0 if n % 4 == 1 a(n) = (-1)^floor((n+2)/4) *
 *            2^floor(n*(n-1)/4) otherwise
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 11, i64 n <= 17, i128 n <= 23,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Compute A160636 at index n (offset 0). */
seq_t a(idx_t n)
{
    /* Size of n is an unsigned integer type (idx_t). */
    if (n % 4 == 1)            /* a(4k+1) = 0 */
        return (seq_t)0;

    /* exponent = floor(n*(n-1)/4) */
    idx_t exp = (n * (n - 1)) / 4;

    /* sign = (-1)^floor((n+2)/4) */
    int sign = ((n + 2) / 4) & 1 ? -1 : 1;

    seq_t val = pow2(exp);          /* 2^exp, exact for arbitrary precision */

    return sign == 1 ? val : -val;
}
