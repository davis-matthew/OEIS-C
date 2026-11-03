/*
 * A211635  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2<x^2+y^2.
 * https://oeis.org/A211635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n³ – Σ_{w=1}^{n} #{(x,y)∈[1,n]² : x² + y² ≤ w²}
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sum counts “bad” triples with w² ≥ x²+y²; subtract from n³. */

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
    if (n == 0)
        return (seq_t)0;

    seq_t n_s  = (seq_t)n;
    seq_t total = n_s * n_s * n_s;   /* n³ */

    seq_t sum_bad = 0;

    for (idx_t w = 1; w <= n; ++w)
    {
        idx_t w2 = w * w;
        idx_t x_max = (n < w) ? n : w;
        seq_t bad_w = 0;

        for (idx_t x = 1; x <= x_max; ++x)
        {
            idx_t diff = w2 - x * x;          /* w² – x² ≥ 0 */
            seq_t ymax = floor_sqrt((seq_t)diff);
            if (ymax > n_s)
                ymax = n_s;
            bad_w += ymax;                     /* y from 1 to ymax */
        }

        sum_bad += bad_w;
    }

    return total - sum_bad;
}
