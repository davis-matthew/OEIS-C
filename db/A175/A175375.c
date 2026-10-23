/*
 * A175375  Number of integer triples (x,y,z) satisfying x^4+y^4+z^4=n, -n <=
 *          x,y,z <= n.
 * https://oeis.org/A175375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 9999, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   intFourthRoot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A175375: Number of integer triples (x,y,z) with x^4+y^4+z^4 = n,
   -n <= x,y,z <= n.  Computed by enumerating all feasible x,y,z. */

#include <stdint.h>
#include <math.h>
#include "../utils/intFourthRoot.h"

/* Count integer triples (x,y,z) with x^4+y^4+z^4 = n, |x|,|y|,|z| <= n. */
seq_t a(idx_t n)
{
    idx_t root = intFourthRoot(n);          /* |x|,|y|,|z| cannot exceed this. */
    seq_t count = 0;

    /* Loop over all feasible x, y, z. The bounds are small (root≈10 for n≤10000). */
    for (int64_t x = -(int64_t)root; x <= (int64_t)root; ++x) {
        seq_t absx = (x >= 0) ? (seq_t)x : -(seq_t)x;
        seq_t x4 = absx * absx * absx * absx;

        for (int64_t y = -(int64_t)root; y <= (int64_t)root; ++y) {
            seq_t absy = (y >= 0) ? (seq_t)y : -(seq_t)y;
            seq_t y4 = absy * absy * absy * absy;
            seq_t xySum = x4 + y4;
            if (xySum > n) continue;            /* no possible z can make sum ≤ n */

            for (int64_t z = -(int64_t)root; z <= (int64_t)root; ++z) {
                seq_t absz = (z >= 0) ? (seq_t)z : -(seq_t)z;
                seq_t z4 = absz * absz * absz * absz;
                if (xySum + z4 == n)
                    ++count;
            }
        }
    }

    return count;
}
