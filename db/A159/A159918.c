/*
 * A159918  Number of ones in binary representation of n^2.
 * https://oeis.org/A159918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = popcount(n^2)
 * Range:     u8 n <= 15, u16 n <= 560, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute n^2 in seq_t, then count the number of 1-bits. */
    seq_t sq   = (seq_t)n * (seq_t)n;
    seq_t count = 0;

    while (sq) {
        sq &= sq - 1;  /* clear the lowest set bit */
        count++;
    }
    return count;
}
