/*
 * A214246  Number A(n,k) of compositions of n where differences between
 *          neighboring parts are in {-k,0,k}; square array A(n,k), n>=0,
 *          k>=0, read by antidiagonals.
 * https://oeis.org/A214246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of compositions of the n-coordinate of the
 *            antidiagonal (n,k) with differences between neighboring parts in
 *            {-k,0,k}. The mapping from the linear index n to the coordinate
 *            (n,k) uses the antidiagonal ordering: n0 = offset, k0 = sum -
 *            offset with sum = floor((sqrt(8*n+1)-1)/2)
 * Range:     u8 n <= 102, u16 n <= 322, u32 n <= 1078, u64 n <= 4092,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 94 terms through n = 8935 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

/* Count compositions of n with step set {-k,0,k} */
static seq_t count_compositions(idx_t n, idx_t k)
{
    if (n == 0) return 1;          /* empty composition */

    /* DP table: dp[sum][last] -- number of ways to reach `sum`
       with last part equal to `last`.  Size is enough for n <= 200. */
    static seq_t dp[200][200];
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = 1; j <= n; ++j)
            dp[i][j] = 0;

    for (idx_t s = 1; s <= n; ++s)          /* single part compositions */
        dp[s][s] = 1;

    for (idx_t sum = 1; sum <= n; ++sum) {
        for (idx_t last = 1; last <= n; ++last) {
            seq_t val = dp[sum][last];
            if (!val) continue;

            /* next part equal to last */
            if (sum + last <= n) dp[sum + last][last] += val;

            if (k != 0) {
                idx_t p = last > k ? last - k : 0;
                if (p > 0 && sum + p <= n) dp[sum + p][p] += val;

                p = last + k;
                if (p <= n && sum + p <= n) dp[sum + p][p] += val;
            }
        }
    }

    seq_t ans = 0;
    for (idx_t last = 1; last <= n; ++last) ans += dp[n][last];
    return ans;
}

/* Helper: integer square root for 64‑bit values */
static inline idx_t int_sqrt_u64(idx_t x)
{
    idx_t r = sqrt((double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    /* Map linear index n to antidiagonal coordinate (n0,k0) */
    idx_t d = 8 * n + 1;
    idx_t sqrt_d = int_sqrt_u64(d);
    idx_t sum = (sqrt_d - 1) / 2;            /* floor of (sqrt(8n+1)-1)/2 */

    /* Adjust if necessary due to flooring */
    while ((sum + 1) * (sum + 2) / 2 <= n) ++sum;
    while (sum * (sum + 1) / 2 > n) --sum;

    idx_t offset = n - sum * (sum + 1) / 2;
    idx_t n0 = offset;
    idx_t k0 = sum - offset;

    return count_compositions(n0, k0);
}
