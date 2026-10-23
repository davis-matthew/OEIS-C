/*
 * A030110  a(n) = 2^n - n^2 + 1.
 * https://oeis.org/A030110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - n^2 + 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);                /* 2^n */
    val -= (seq_t)n * (seq_t)n;         /* subtract n^2 */
    val += (seq_t)1;                    /* add 1 */
    return val;
}
