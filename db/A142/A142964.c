/*
 * A142964  a(n) = 6*2^n - 2*n - 5.
 * https://oeis.org/A142964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*2^n - 2*n - 5
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = 6 * pow2(n);          /* 6 * 2^n */
    val -= (seq_t)2 * (seq_t)n;       /* subtract 2*n */
    val -= 5;                         /* subtract 5 */
    return val;
}
