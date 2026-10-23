/*
 * A036102  Centered cube numbers: (n+1)^24 + n^24.
 * https://oeis.org/A036102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^24 + n^24
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 5, u128 n <= 38,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t b1 = (seq_t)n + (seq_t)1;
    seq_t b0 = (seq_t)n;

    seq_t p1 = seq_pow(b1, 24);
    seq_t p0 = seq_pow(b0, 24);

    return p1 + p0;
}
