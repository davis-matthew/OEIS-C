/*
 * A211810  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2w^2>x^2+y^2.
 * https://oeis.org/A211810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(w,x,y) : 1≤w,x,y≤n and 2*w^2 > x^2 + y^2}
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t w = 1; w <= n; ++w) {
        /* 2*w^2 as seq_t to avoid overflow in 32‑bit builds */
        seq_t two_w2 = (seq_t)w * w * 2;

        /* If 2*w^2 is too small there can be no y at all */
        if (two_w2 < 2) continue;

        /* Initial maximum y for x = 1 : need y^2 < 2*w^2 - 1
           hence y_max = floor_sqrt(2*w^2 - 2)                 */
        idx_t y_max = (idx_t)floor_sqrt(two_w2 - 2);

        /* Loop over x; stop when x^2 >= 2*w^2 (then M ≤ 0) */
        for (idx_t x = 1; x <= n; ++x) {
            seq_t x_sq = (seq_t)x * x;
            if (x_sq >= two_w2) break;          /* no further valid x */

            seq_t M = two_w2 - x_sq;            /* M = 2*w^2 - x^2  , M > 0 */

            if (M <= 1) continue;              /* y^2 < M requires M ≥ 2 */

            /* Decrease y_max until y_max^2 < M  (y must satisfy y^2 < M) */
            while (y_max > 0 && (seq_t)y_max * y_max >= M) {
                --y_max;
            }

            /* y must also lie in 1..n */
            idx_t add = y_max > n ? n : y_max;
            total += (seq_t)add;
        }
    }
    return total;
}
