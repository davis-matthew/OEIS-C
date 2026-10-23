/*
 * A036703  a(n)=number of Gaussian integers z=a+bi satisfying n-1<|z|<=n,
 *          a>=0, 0<=b<=a.
 * https://oeis.org/A036703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of Gaussian integers z=a+bi with n-1 < |z| ≤ n,
   a ≥ 0, 0 ≤ b ≤ a  (formula this function implements).  */
#include <stdint.h>

/* Integer floor square root of a 64‑bit unsigned value.
   Uses binary search and only integer arithmetic.  */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            ans = 0;
            low = 1;
            continue;
        }
        if (mid <= x / mid) {          /* mid*mid <= x  */
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Special case n = 0: only the zero Gaussian integer satisfies the
       inequality.  */
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute the squared bounds.  These avoid overflow for the
       intended range of n (tested up to at least 10 000).  */
    idx_t r_sq = n * n;
    idx_t l_sq = (n - 1) * (n - 1);

    seq_t total = (seq_t)0;

    /* For each possible real part a (0 ≤ a ≤ n) count admissible b.  */
    for (idx_t a = 0; a <= n; ++a) {
        idx_t a2 = a * a;
        if (a2 > r_sq)
            break;                /* further a would exceed |z| ≤ n */

        /* Upper bound for |z|: a^2 + b^2 ≤ n^2  →  b^2 ≤ r_sq - a2   */
        idx_t bmax_sq = r_sq - a2;
        idx_t bmax = isqrt_u64(bmax_sq);
        if (bmax > a)
            bmax = a;             /* enforce b ≤ a */

        /* Lower bound for |z|: a^2 + b^2 > l_sq  →  b^2 > l_sq - a2   */
        idx_t bmin;
        if (a2 > l_sq) {          /* inequality already satisfied */
            bmin = 0;
        } else {
            idx_t lim = l_sq - a2;
            bmin = isqrt_u64(lim) + 1;    /* smallest b with b^2 > lim */
        }

        if (bmin > a)            /* no admissible b for this a     */
            continue;

        /* Count admissible values within [bmin , min(bmax,a)] */
        qty: ({
            idx_t upper = bmax;
            if (upper > a)
                upper = a;
            if (upper >= bmin)
                total += (seq_t)(upper - bmin + 1);
        });
    }

    return total;
}
