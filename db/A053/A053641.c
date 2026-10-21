/*
 * A053641  Rotate n one binary digit to the right, drop leading zeros, then
 *          rotate one binary digit to the left.
 * https://oeis.org/A053641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n if n is odd * a(n) = n - 2^⌊log2(n)⌋ + 1 if n is even *
 *            (which is equivalent to rotate the binary representation of n *
 *            one bit to the right, drop leading zeros, then rotate one bit *
 *            to the left)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset is 1, so n >= 1.  The function is defined for any n>=1. */
    if ((n & 1ULL) != 0ULL)          /* n is odd */
        return (seq_t)n;

    /* n is even.  Find k = floor(log2(n)).  Since n <= 2^64-1,
       k fits into a 64‑bit integer. */
    idx_t k = 0;
    idx_t t = n >> 1;                /* shift until zero to count bits */
    while (t > 0) {
        ++k;
        t >>= 1;
    }

    /* 2^k as seq_t.  This shift is safe because k <= 63 and
       seq_t has at least 8 bits. */
    seq_t pow2k = ((seq_t)1) << k;

    return (seq_t)n - pow2k + 1;
}
