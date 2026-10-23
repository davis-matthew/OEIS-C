/*
 * A172160  a(0)=1. a(n) = 2^(n-2)*(5-n), for n>0.
 * https://oeis.org/A172160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 for n=0, 2 for n=1, a(n) = 2^(n-2) * (5-n) for n>1
 *            (OEIS A172160)
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 28, i64 n <= 59, i128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    seq_t pow = pow2(n - 2);               /* 2^(n-2) */
    seq_t factor = (seq_t)5 - (seq_t)n;     /* (5-n) */
    return pow * factor;                   /* 2^(n-2)*(5-n) */
}
