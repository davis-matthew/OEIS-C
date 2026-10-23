/*
 * A153142  Permutation of nonnegative integers: A059893-conjugate of A153152.
 * https://oeis.org/A153142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = flip bits from the second most significant one down to
 *            the first zero (inclusive)
 * Range:     u8 n <= 255, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Find the index of the most significant bit, h (0‑based). */
    idx_t h = 0;
    idx_t t = n;
    while (t > 1) {
        t >>= 1;
        ++h;
    }

    seq_t res = (seq_t)n;
    for (idx_t i = h; i > 0; --i) {
        idx_t bit = (n >> (i - 1)) & 1;
        res ^= ((seq_t)1 << (unsigned)(i - 1));
        if (bit == 0)
            break;
    }
    return res;
}
