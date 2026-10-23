/*
 * A014081  a(n) is the number of occurrences of '11' in the binary expansion
 *          of n.
 * https://oeis.org/A014081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of occurrences of the pattern “11” in the binary
 *            expansion of n. * Equivalently, a(n) = popcount((n >> 1) & n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* shift right by 1 to align overlapping pairs of bits */
    idx_t shifted = n >> 1;
    /* mask gives a 1 where both the original bit and its neighbour are 1 */
    idx_t pairs   = n & shifted;
    seq_t count   = 0;
    /* popcount of pairs */
    while (pairs) {
        pairs &= pairs - 1; /* clear lowest set bit */
        ++count;
    }
    return count;
}
