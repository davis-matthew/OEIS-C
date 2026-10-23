/*
 * A183633  Number of (n+1) X (n+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to 6.
 * https://oeis.org/A183633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*6^n - 12*3^n + 4*2^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Undefined for n == 0; return 0 to avoid undefined behaviour. */
    if (n == 0u) return 0;

    /* Compute the needed powers using the utility function seq_pow. */
    seq_t pow6 = seq_pow((seq_t)6, n);   /* 6^n  */
    seq_t pow3 = seq_pow((seq_t)3, n);   /* 3^n  */
    seq_t pow2 = seq_pow((seq_t)2, n);   /* 2^n  */

    /* Combine them according to the closed formula. */
    seq_t res = (seq_t)12 * pow6;
    res -= (seq_t)12 * pow3;
    res += (seq_t)4 * pow2;

    return res;
}
