/*
 * A211340  Number of integer pairs (x,y) such that 1<x<=y<=n and
 *          x^2+y^2<=n^2.
 * https://oeis.org/A211340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y) : 1 ≤ x ≤ y ≤ n and x² + y² ≤ n²}
 * Range:     u8 n <= 8, u16 n <= 54, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 70 terms through n = 1639 (2026-11-01)
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
    if (n < 1) return 0;          /* undefined for n < 1, but return 0 */

    seq_t count   = 0;
    seq_t n_seq   = (seq_t)n;
    seq_t n_sq    = n_seq * n_seq; /* n² */

    for (idx_t x = 1; x <= n; ++x)
    {
        seq_t xsq = (seq_t)x * (seq_t)x;   /* x² */
        if (xsq > n_sq) break;             /* not needed, but safe */

        seq_t rem     = n_sq - xsq;          /* n² − x² */
        seq_t ylimit  = floor_sqrt(rem);     /* floor(sqrt(rem)) */

        if (ylimit < (seq_t)x) continue;     /* no y with y ≥ x */

        /* y must also be ≤ n */
        seq_t ymax = (ylimit <= n_seq ? ylimit : n_seq);

        count += ymax - (seq_t)x + 1;        /* count y in [x, ymax] */
    }

    return count;
}
