/*
 * A188162  a(n) = ceiling( 4^n/20 - (n^2 + 1)*2^(n-1) ).
 * https://oeis.org/A188162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ceil(4^n/20 - (n^2 + 1)*2^(n-1))
 * Range:     i8 n <= 1, i16 n <= 6, i32 n <= 15, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static seq_t ceil_div_seq(seq_t a, seq_t b)
{
    /* Ceil division for positive divisor b */
    if (a % b == 0) return a / b;
    if (a > 0)       return a / b + 1;
    return a / b;    /* a < 0, truncating division already gives ceil */
}

seq_t a(idx_t n)
{
    seq_t B  = pow2(n);          /* 2^n */
    seq_t A  = B * B;            /* 4^n = (2^n)^2 */
    seq_t ns = (seq_t)n;
    seq_t n_sq = ns * ns;        /* n^2 */
    seq_t tmp = n_sq + 1;        /* n^2 + 1 */
    tmp = tmp * 10;
    tmp = tmp * B;               /* 10*(n^2+1)*2^n */
    seq_t N = A - tmp;           /* numerator of the fraction */
    return ceil_div_seq(N, 20);
}
