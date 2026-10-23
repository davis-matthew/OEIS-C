/*
 * A134914  a(n) = ceiling(n^(1/3)).
 * https://oeis.org/A134914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^(1/3))
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard against 0 */
    seq_t x = (seq_t)n;
    seq_t r = floor_cube_root(x);
    seq_t r3 = r * r * r;
    if (r3 < x) return r + ((seq_t)1);
    return r;
}
