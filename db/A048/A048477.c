/*
 * A048477  a(n) = T(5,n), array T given by A048472.
 * https://oeis.org/A048477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*2^n + 1 (equivalent to 6*n*2^(n-1)+1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t pow2 = seq_pow((seq_t)2, n);

    /* 3*n */
    seq_t three_n = (seq_t)n * 3;

    /* 3*n*2^n + 1 */
    seq_t val = pow2 * three_n + (seq_t)1;

    return val;
}
