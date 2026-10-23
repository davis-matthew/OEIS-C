/*
 * A164705  T(n,k) = binomial(2n-k,n) * 2^(k-1), T(0,0)=1; triangle T(n,k),
 *          n>=0, 0<=k<=n, read by rows.
 * https://oeis.org/A164705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(2*m, m)/2 if r = 0 or r = 1 * a(n) = C(2*m - r, m - r)
 *            * 2^(r-1) otherwise * where m = ⌊(√(8*n + 1) – 1)/2⌋ (the
 *            largest m with m(m+1)/2 ≤ n) * and r = n – m(m + 1)/2 (offset of
 *            n inside its triangle block)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"   /* binom(idx_t, idx_t) --> seq_t */
#include "../utils/pow2.h"    /* pow2(idx_t)  --> seq_t */

/* integer square root: returns ⌊√x⌋  (works for 64‑bit values) */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid == 0) {   /* avoid division by zero in the next test */
            low = 1;
            continue;
        }
        if (mid <= x / mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* a(0) = 1 */

    /* find m such that T(m) = m(m+1)/2 ≤ n < T(m+1) */
    idx_t s = isqrt_u64(8 * n + 1);     /* floor(√(8n+1)) */
    idx_t m = (s - 1) >> 1;             /* (s-1)/2 */

    idx_t tri   = m * (m + 1) / 2;      /* T(m) */
    idx_t r     = n - tri;              /* offset inside block */

    if (m == 0) return (seq_t)1;        /* only possible for n = 0 */

    seq_t base = binom(2 * m, m) >> 1;  /* C(2m,m)/2 */

    if (r <= 1) return base;           /* r = 0 or 1 → central term */

    /* r ≥ 2 */
    idx_t k  = m - r;                   /* m - r */
    seq_t t  = binom(2 * m - r, k);     /* C(2m - r, m - r) */
    seq_t p  = pow2(r - 1);             /* 2^(r-1) */
    return t * p;
}
