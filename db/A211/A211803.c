/*
 * A211803  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2w^2>=x^2+y^2.
 * https://oeis.org/A211803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For each w = 1..n count ordered pairs (x,y) with 1≤x,y≤n and x²
 *            + y² ≤ 2w², then sum over w. This is exactly the definition of
 *            sequence A211803
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 64‑bit unsigned number: floor(sqrt(n)) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t total = 0;

    for (idx_t w = 1; w <= n; ++w)
    {
        idx_t limit = 2 * w * w;              /* 2w^2  (fits in 64‑bit for n≤10000) */
        idx_t maxx  = isqrt_u64(limit);
        if (maxx > n) maxx = n;

        /* initial y for the first x=1 */
        idx_t y = isqrt_u64(limit - 1);
        if (y > n) y = n;

        for (idx_t x = 1; x <= maxx; ++x)
        {
            idx_t xsq = x * x;
            /* shrink y until x^2 + y^2 ≤ limit */
            while (y > 0 && xsq + y * y > limit)
                --y;
            total += (seq_t)y;                 /* count y = 1..y */
        }
    }
    return total;
}
