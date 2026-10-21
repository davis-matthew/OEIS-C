/*
 * A053644  Most significant bit of n, msb(n); largest power of 2 less than or
 *          equal to n; write n in binary and change all but the first digit
 *          to zero.
 * https://oeis.org/A053644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2^⌊log₂(n)⌋ (a(0) = 0)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest power of 2 ≤ n, or 0 for n = 0.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* find the highest power of 2 not exceeding n.
       Use a loop that never overflows even when n is close to UINT64_MAX. */
    idx_t r = 1;
    while (r <= (n >> 1))
        r <<= 1;

    return (seq_t)r;
}
