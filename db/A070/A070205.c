/*
 * A070205  Number of acute integer triangles with perimeter n having integral
 *          inradius.
 * https://oeis.org/A070205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of acute integer triangles with perimeter n having
 *            integral inradius
 * Range:     u8 n <= 39, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates the equivalent (x,y,z) triple
 * representation of an integer triangle:
 *
 *   a = y+z,   b = z+x,   c = x+y   (x,y,z > 0,  x+y+z = s = n/2)
 *
 * The triangle is acute iff
 *   z^2 + zy + zx > xy,
 *   x^2 + xz + xy > yz,
 *   y^2 + yz + yx > zx.
 *
 * Integral inradius r follows from
 *   r = sqrt(x*y*z / s).
 * Thus we require (x*y*z) divisible by s and the quotient to be a perfect square.
 *
 * Only even n can produce such triangles (s must be integer). For odd n the
 * function returns 0.
 *
 * The loop iterates only over unordered triples x ≤ y ≤ z,
 * guaranteeing each triangle is counted once.  Complexity is
 * O((n/2)^2) per call, easily below the 2‑second limit for n≤10000. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Return 0 for odd perimeter; no triangle can have
     * a half‑perimeter s non‑integer. */
    if (n & 1ULL) return (seq_t)0;

    idx_t s = n >> 1;          /* s = n/2,  integer */
    seq_t count = (seq_t)0;

    if (s < 3) return count;   /* need at least 3 units for positive x,y,z */

    for (idx_t x = 1; x <= s - 2; ++x) {
        idx_t max_y = s - x - 1;   /* ensure z >= 1 */
        for (idx_t y = x; y <= max_y; ++y) {
            idx_t z = s - x - y;
            if (z < y) break;      /* maintain x <= y <= z */

            /* Acute‑triangle test (symmetric inequalities) */
            if ((z*z + z*y + z*x) <= (x*y)) continue;
            if ((x*x + x*z + x*y) <= (y*z)) continue;
            if ((y*y + y*z + y*x) <= (z*x)) continue;

            /* Integral inradius test */
            seq_t prod = (seq_t)x * (seq_t)y * (seq_t)z;
            if (prod % s != 0) continue;
            seq_t t = prod / s;
            seq_t sq = floor_sqrt(t);
            if (sq * sq != t) continue;

            ++count;
        }
    }
    return count;
}
