/*
 * A188129  a(n) = (2^n+3)^2-8.
 * https://oeis.org/A188129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n+3)^2-8
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow2 = seq_pow((seq_t)2, n);      /* 2^n  */
    seq_t sum = pow2 + (seq_t)3;             /* 2^n+3 */
    seq_t sq  = sum * sum;                   /* (2^n+3)^2 */
    return sq - (seq_t)8;                    /* (2^n+3)^2-8 */
}
