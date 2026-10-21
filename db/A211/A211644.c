/*
 * A211644  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2+x^2+y^2>2n.
 * https://oeis.org/A211644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(w,x,y)∈{1..n}^3 | w^2 + x^2 + y^2 > 2n}
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: count all n^3 triples and subtract those with w^2+x^2+y^2 ≤ 2n. */
/* Uses an integer square‑root that works for arbitrary 64‑bit unsigned integers. */
#include <stdint.h>

/* Integer square root: floor(sqrt(v)) for 0 ≤ v ≤ UINT64_MAX. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v, res = 0;
    while (lo <= hi)
    {
        idx_t mid = (lo + hi) >> 1;
        if (mid > 0 && mid > v / mid)
            hi = mid - 1;
        else
        {
            res = mid;
            lo = mid + 1;
        }
    }
    return res;
}

/* Sequence entry %idx_t, result %seq_t. */
seq_t a(idx_t n)
{
    /* Total number of ordered triples in [1..n]^3. */
    seq_t total = (seq_t)n * (seq_t)n * (seq_t)n;
    if (n == 0) return 0;                      /* a(0) = 0 */

    /* Precompute 2n safely (prevent overflow). */
    idx_t two_n = (n <= (UINT64_MAX >> 1)) ? n * 2 : UINT64_MAX;

    /* Limit for w: w^2 ≤ 2n.  w cannot exceed n anyway. */
    idx_t w_limit = isqrt_u64(two_n);
    if (w_limit > n) w_limit = n;

    idx_t count_small = 0;                      /* triples with sum ≤ 2n */

    for (idx_t w = 1; w <= w_limit; ++w)
    {
        idx_t w2 = w * w;
        idx_t R = two_n - w2;                   /* remaining for x^2 + y^2 */

        /* Limit for x: x^2 ≤ R.  x cannot exceed n. */
        idx_t x_limit = isqrt_u64(R);
        if (x_limit > n) x_limit = n;

        for (idx_t x = 1; x <= x_limit; ++x)
        {
            idx_t x2   = x * x;
            idx_t rem = R - x2;                 /* remaining for y^2 */
            if (rem == 0) continue;             /* y must be at least 1 */

            idx_t ymax = isqrt_u64(rem);
            if (ymax > n) ymax = n;

            count_small += ymax;                /* add all valid y */
        }
    }

    seq_t small = (seq_t)count_small;
    return total - small;                       /* a(n) = total - small */
}
