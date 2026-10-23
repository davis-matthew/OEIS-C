/*
 * A155031  Triangle T(n, k) = 0 if n==0 (mod k) otherwise -1 with T(n, n) = 1
 *          and T(n, 0) = 0, read by rows.
 * https://oeis.org/A155031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r is the smallest integer with r(r+1)/2 ≥ n
 *            and k = n – r(r–1)/2. T(r,k) = 1 if k = r, 0 if r mod k = 0, –1
 *            otherwise
 * Range:     i8 n <= 465, i16 n <= 465, i32 n <= 465, i64 n <= 465,
 *            i128 n <= 465, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*=== Utilities =============================================================*/
#include <stdint.h>

/* Integer square root: largest s with s*s ≤ x.  Uses binary search. */
static inline idx_t _isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, mid, ans = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        if (mid <= x / mid) {    /* mid*mid ≤ x, avoid overflow */
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/*=== Sequence function =====================================================*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for the offset, return 0 */

    /* Find row r: r(r+1)/2 ≥ n */
    idx_t t   = 8 * n + 1;           /* 8n+1 is always positive */
    idx_t s   = _isqrt_u64(t);
    idx_t r   = (s - 1) >> 1;        /* floor((sqrt(8n+1)-1)/2) */
    /* Adjust if the bound is too small */
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Compute column k */
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t k = n - prev_sum; /* 1 ≤ k ≤ r */

    /* Triangle entry T(r,k) */
    if (k == r)              return (seq_t)1;
    if (r % k == 0)          return (seq_t)0;
    return (seq_t)-1;
}
