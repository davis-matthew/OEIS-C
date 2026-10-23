/*
 * A159964  a(n) = 2^n*(1-n).
 * https://oeis.org/A159964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2^n*(1-n)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 26, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t power = pow2(n);              /* 2^n */
    seq_t coeff = (seq_t)1 - (seq_t)n;   /* 1 - n */
    return power * coeff;               /* 2^n * (1 - n) */
}
