/*
 * A025455  a(n) is the number of partitions of n into 2 positive cubes.
 * https://oeis.org/A025455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (x,y) of positive integers with x^3 + y^3
 *            = n, x ≥ y > 0
 * Range:     u8 n <= 64, u16 n <= 9923, u32 n <= 9923, u64 n <= 9923,
 *            u128 n <= 9923, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* n = 0 or 1 cannot be written as a sum of two positive cubes */
    if (n < 2)
        return 0;

    /* y must satisfy y^3 ≤ n/2, so y ≤ floor(cbrt(n/2)) */
    idx_t half   = n >> 1;                       /* integer division by 2 */
    idx_t max_y  = (idx_t)floor_cube_root((seq_t)half);

    for (idx_t y = 1; y <= max_y; ++y) {
        seq_t y3 = (seq_t)y * y * y;
        if (y3 > (seq_t)n)
            break;                               /* safety guard */

        seq_t remaining = (seq_t)n - y3;
        seq_t rseq      = floor_cube_root(remaining);
        idx_t r         = (idx_t)rseq;

        if ((seq_t)r * r * r == remaining && r >= y)
            ++count;
    }

    return count;
}
