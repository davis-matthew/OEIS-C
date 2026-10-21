/*
 * A058962  a(n) = 2^(2*n)*(2*n+1).
 * https://oeis.org/A058962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n)*(2*n+1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 4^n */
    seq_t pow4 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
    {
        pow4 *= (seq_t)4;
    }

    /* Compute (2*n + 1) */
    seq_t two_n = (seq_t)n * 2;
    seq_t term = two_n + (seq_t)1;

    return pow4 * term;
}
