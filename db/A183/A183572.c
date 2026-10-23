/*
 * A183572  a(n) = n + floor(sqrt(2*n-1)).
 * https://oeis.org/A183572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(2*n-1))
 * Range:     u8 n <= 32, u16 n <= 7287, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; returning 0 for n == 0 is harmless. */
    if (n == 0) return 0;
    return (seq_t)n + floor_sqrt(((seq_t)n * 2) - 1);
}
