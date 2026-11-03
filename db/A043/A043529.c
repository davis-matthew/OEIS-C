/*
 * A043529  Number of distinct base-2 digits of n.
 * https://oeis.org/A043529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 if (n & (n+1)) == 0, else 2. Equivalent to 2 -
 *            A036987(n)
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 107 terms through n = 9992 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    return ((n & (n + 1)) == 0) ? (seq_t)1 : (seq_t)2;
}
