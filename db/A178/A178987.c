/*
 * A178987  a(n) = n*(n-3)*2^(n-2).
 * https://oeis.org/A178987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(n-3)*2^(n-2) for n >= 3; a(0) = 0, a(1) = -1, a(2) =
 *            -2
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 24, i64 n <= 53, i128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)-2;
    seq_t res = pow2(n-2);          /* 2^(n-2) */
    res *= (seq_t)n;                /* * n */
    res *= (seq_t)(n-3);            /* * (n-3) */
    return res;
}
