/*
 * A211641  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2+x^2+y^2>=n.
 * https://oeis.org/A211641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Count of ordered triples (w,x,y) with 1 ≤ w,x,y ≤ n and w² + x²
 *            + y² ≥ n
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static inline idx_t isqrt_floor(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > v / mid)   /* mid*mid would overflow or > v */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Allocate arrays for squares and y‑counts */
    idx_t *sq  = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    idx_t *cnt = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!sq || !cnt) { /* allocation failure: return 0 as safe fallback */
        free(sq);
        free(cnt);
        return 0;
    }

    /* Precompute squares */
    for (idx_t i = 1; i <= n; ++i)
        sq[i] = i * i;

    /* Precompute number of y values for each possible threshold t */
    cnt[0] = n;  /* all y satisfy when threshold ≤ 0 */
    for (idx_t t = 1; t <= n; ++t)
    {
        idx_t s = isqrt_floor(t);
        if (s * s < t) ++s;                 /* ceil(sqrt(t)) */
        if (s <= n)
            cnt[t] = (n - s + 1);           /* y from s to n */
        else
            cnt[t] = 0;
    }

    seq_t total = 0;

    /* Count triples */
    for (idx_t w = 1; w <= n; ++w)
    {
        idx_t w2 = sq[w];
        for (idx_t x = 1; x <= n; ++x)
        {
            idx_t sum = w2 + sq[x];
            if (sum >= n)
                total += (seq_t)n;          /* all y satisfy */
            else
            {
                idx_t rem = (idx_t)n - sum;  /* positive threshold */
                total += (seq_t)cnt[rem];
            }
        }
    }

    free(sq);
    free(cnt);
    return total;
}
