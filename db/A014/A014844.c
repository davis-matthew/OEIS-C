/*
 * A014844  a(n) = 2^n - n*(n-1)/2.
 * https://oeis.org/A014844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - n*(n-1)/2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t p = pow2(n);

    /* binomial term n choose 2 */
    seq_t bin = 0;
    if (n > 1)
        bin = (seq_t)n * ((seq_t)n - 1) / 2;

    return p - bin;
}
