/*
 * A089216  Number of triples (x,y,z) of integers such that 0 <= x <= y < n, 0
 *          <= z < n, n^2 = x^2 + y^2 - z^2.
 * https://oeis.org/A089216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) of integers such that 0 ≤ x ≤ y
 *            < n, 0 ≤ z < n, n² = x² + y² – z²
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   isqrt_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/isqrt_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t count = 0;
    idx_t n2 = n * n;

    for (idx_t z = 0; z < n; ++z) {
        idx_t t = n2 + z * z;               /* target x² + y² */
        for (idx_t x = 0; x < n; ++x) {
            idx_t sqx = x * x;
            if (sqx > t) break;
            idx_t val = t - sqx;
            idx_t y = isqrt_idx(val);
            if (y * y != val) continue;    /* not a square */
            if (y < x) continue;           /* enforce x ≤ y */
            if (y >= n) continue;          /* y must be < n */
            ++count;
        }
    }
    return count;
}
