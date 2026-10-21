/*
 * A211643  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2+x^2+y^2<=2n.
 * https://oeis.org/A211643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # {(w,x,y) ∈ [1..n]^3 : w² + x² + y² ≤ 2n}
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts ordered triples with squared‐sum bounded by 2n.  */
/* Implementation uses only integer arithmetic; the triple loop is
   reduced by counting permissible y via an integer square root. */

#include <stdint.h>   /* for 0xFFFFFFFFULL constant */

/* Integer square root for unsigned 64‑bit values. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = 0xFFFFFFFFULL;    /* hi = ⌊√(2^64‑1)⌋ */
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t midsq = mid * mid;
        if (midsq == n)
            return mid;
        else if (midsq < n)
            lo = mid + 1;
        else
        {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* a(0) = a(1) = 0 */

    idx_t limitSq = 2 * n;                /* upper bound for w²+x²+y² */
    idx_t m = isqrt_u64(limitSq);        /* w,x,y cannot exceed √(2n) */

    seq_t count = 0;

    for (idx_t w = 1; w <= m; ++w)
    {
        idx_t w2 = w * w;
        for (idx_t x = 1; x <= m; ++x)
        {
            idx_t sum_wx = w2 + x * x;
            if (sum_wx > limitSq) /* inner loop can be skipped */
                continue;

            idx_t rem = limitSq - sum_wx;          /* remaining budget */
            idx_t maxY = isqrt_u64(rem);           /* y² ≤ rem */
            if (maxY > m)                          /* y also bounded by m */
                maxY = m;

            count += (seq_t)maxY;                  /* all admissible y */
        }
    }
    return count;
}
