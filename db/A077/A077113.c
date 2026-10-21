/*
 * A077113  Number of nonnegative integer cubes <= n^2.
 * https://oeis.org/A077113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(2/3)) + 1
 * Range:     u8 n <= 7, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the least m such that m^3 > n^2. */
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t r  = floor_cube_root(sq);
    return r + 1;
}
