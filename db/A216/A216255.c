/*
 * A216255  Triangle read by rows: T(n,k) is the number of labeled rooted
 *          trees of height at most 2 that have exactly k nodes at a distance
 *          2 from the root; n>=1, 0<=k<=n-1.
 * https://oeis.org/A216255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the triangle read by rows: if n lies in row r and
 *            position k (0‑based) of that row, then a(n) = T(r,k) where
 *            T(n,k) = n * C(n-1,k) * (n-k-1)^k. C(n,k) is the binomial
 *            coefficient. The function assumes the existence of seq_t
 *            binomial(idx_t a, idx_t b); (from ../utils/binomial.h) and that
 *            se...
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 98, u64 n <= 266,
 *            u128 n <= 764, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static seq_t power_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    if (exp == 0) return 1;
    for (idx_t i = 0; i < exp; ++i)
        result = result * base;
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row r (1‑based) containing index n.  */
    idx_t lo = 1, hi = 1;
    while ((hi * (hi + 1)) / 2 < n) hi <<= 1;          /* exponential search */
    while (lo < hi)                                 /* binary search */
    {
        idx_t mid = lo + (hi - lo) / 2;
        if ((mid * (mid + 1)) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;   /* row number (1-based) */

    /* Compute column index k (0‑based) within the row */
    idx_t row_start = (r - 1) * r / 2 + 1;
    idx_t k = n - row_start;  /* 0 ≤ k < r */

    /* Compute T(r,k) = r * C(r-1,k) * (r-k-1)^k */
    seq_t bin = binomial(r - 1, k);             /* C(r-1,k) */
    seq_t base = (seq_t)(r - k - 1);            /* (r-k-1) */
    seq_t pow = power_seq_t(base, k);           /* (r-k-1)^k */
    return (seq_t)r * bin * pow;
}
