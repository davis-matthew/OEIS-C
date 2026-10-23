/*
 * A119869  Sizes of successive clusters in f.c.c. lattice centered at a
 *          lattice point.
 * https://oeis.org/A119869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   a(n) = # of integer triples (x,y,z) with x + y + z even and x² +
 *            y² + z² ≤ 2n
 * Range:     u8 n <= 12, u16 n <= 494, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* total count of lattice points satisfying the conditions */
    seq_t count = 0;

    /* limit on squared distance and its integer square root */
    idx_t limit2 = (idx_t)2 * n;              /* (2·n) fits easily in idx_t */
    idx_t r = _floor_isqrt_u64(limit2);       /* floor(sqrt(2·n))   */
    int r_int = (int)r;                       /* for signed loop limits */

    for (int x = -r_int; x <= r_int; ++x) {
        int64_t x2 = (int64_t)x * x;
        if (x2 > (int64_t)limit2) continue;

        for (int y = -r_int; y <= r_int; ++y) {
            int64_t y2 = (int64_t)y * y;
            int64_t sumxy = x2 + y2;
            if (sumxy > (int64_t)limit2) continue;

            int parity_xy = (x + y) & 1;           /* parity of x + y */

            int64_t remaining = (int64_t)limit2 - sumxy;
            idx_t rem = (idx_t)remaining;
            int zmax = (int)_floor_isqrt_u64(rem);

            for (int z = -zmax; z <= zmax; ++z) {
                if ((z & 1) != parity_xy) continue;
                count += 1;
            }
        }
    }

    return count;
}
