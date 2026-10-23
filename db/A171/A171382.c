/*
 * A171382  a(n) = (2*2^n+7*(-1)^n)/3.
 * https://oeis.org/A171382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*2^n+7*(-1)^n)/3
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 29, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p = pow2(n);                        /* 2^n */
    seq_t s = (n & 1) ? (seq_t)-1 : (seq_t)1; /* (-1)^n   */
    seq_t num = (seq_t)2 * p + (seq_t)7 * s;   /* 2*2^n + 7*(-1)^n */
    return num / (seq_t)3;                    /* divide by 3 */
}
