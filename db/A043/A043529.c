/*
 * A043529  Number of distinct base-2 digits of n.
 * https://oeis.org/A043529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 if (n & (n+1)) == 0, else 2. Equivalent to 2 -
 *            A036987(n)
 * Range:     u8 n <= 9992, u16 n <= 9992, u32 n <= 9992, u64 n <= 9992,
 *            u128 n <= 9992, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    return ((n & (n + 1)) == 0) ? (seq_t)1 : (seq_t)2;
}
