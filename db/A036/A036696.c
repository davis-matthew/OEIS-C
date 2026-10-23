/*
 * A036696  a(n)=number of Gaussian integers z=a+bi satisfying n-1<|z|<=n,
 *          b>=0.
 * https://oeis.org/A036696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Gaussian integers z = a + bi satisfying n-1 <
 *            |z| <= n, with b >= 0
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root for a 64‑bit unsigned integer.
   Uses binary search and division to avoid overflow. */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;   /* upper mid */
        if (mid > 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 */
    if (n == 0)
        return 1;

    idx_t n_sq  = n * n;
    idx_t sq1   = (n - 1) * (n - 1);  /* (n-1)^2 */
    seq_t total = 0;

    for (idx_t b = 0; b <= n; ++b) {
        idx_t bb = b * b;
        if (bb > n_sq)            /* further values of b give no points */
            break;

        idx_t max_a = floor_sqrt_u64(n_sq - bb);  /* |a| <= max_a */

        idx_t min_a;
        if (bb > sq1) {           /* lower bound is automatically satisfied */
            min_a = 0;
        } else {
            idx_t diff = sq1 - bb;           /* (n-1)^2 - b^2 >= 0 */
            min_a = floor_sqrt_u64(diff) + 1;   /* ceil_sqrt(diff + 1) */
        }

        if (max_a < min_a)
            continue;            /* empty range for this b */

        if (min_a == 0) {
            /* a = 0 contributes one point; each |a| > 0 has ±a */
            total += (seq_t)(1 + 2 * max_a);
        } else {
            total += (seq_t)(2 * (max_a - min_a + 1));
        }
    }

    return total;
}
