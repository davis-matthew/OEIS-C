/*
 * A028953  Theta series of quadratic form (or lattice) with Gram matrix [ 3,
 *          1; 1, 4 ].
 * https://oeis.org/A028953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y) ∈ ℤ² : 3*x² + 2*x*y + 4*y² = n}
 * Range:     u8 n <= 5, u16 n <= 104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t N = (seq_t)n;

    /* ymax²  ≤ (3/11)·N  →  |y| ≤ √((3/11)·N) */
    seq_t ymax_sq = (3 * N) / 11;
    seq_t ymax = floor_sqrt(ymax_sq);

    seq_t count = 0;
    int64_t ymax_i = (int64_t)ymax;

    for (int64_t y = -ymax_i; y <= ymax_i; ++y)
    {
        int64_t yy = y;
        seq_t yy2 = (seq_t)(yy * yy);

        /* D = 12·N – 44·y²  must be a perfect square */
        seq_t D = (12 * N) - (44 * yy2);
        if (D < 0) continue;

        seq_t sD = floor_sqrt(D);
        if (sD * sD != D) continue;

        int64_t sD64 = (int64_t)sD;

        /* x = (-2y ± √D) / 6  should be integer */
        int64_t num1 = -2 * yy + sD64;
        int64_t num2 = -2 * yy - sD64;

        if (num1 % 6 == 0)
            ++count;
        /* avoid double‑counting when √D = 0 (both numerators equal) */
        if (sD64 != 0 && num2 % 6 == 0)
            ++count;
    }

    return count;
}
