/*
 * A176778  a(n) = 2^n - n*(n+2).
 * https://oeis.org/A176778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - n*(n+2)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t for calculations */
    seq_t sn = (seq_t)n;
    /* 2^n */
    seq_t pow = pow2(n);
    /* n*(n+2) */
    seq_t prod = sn * (sn + 2);
    return pow - prod;
}
