/*
 * A156067  a(0)=1. a(n)= -2^(n-1)-3*(-1)^n, n>1.
 * https://oeis.org/A156067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1; a(n) = -2^(n-1)-3*(-1)^n for n>0
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t two = pow2(n - 1);        /* 2^(n-1) */
    seq_t sgn = (n & 1) ? -1 : 1;   /* (-1)^n */
    return -two - 3 * sgn;
}
