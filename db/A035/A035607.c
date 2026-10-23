/*
 * A035607  Table a(d,m) of number of points of L1 norm m in cubic lattice
 *          Z^d, read by antidiagonals (d >= 1, m >= 0).
 * https://oeis.org/A035607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 111, u32 n <= 390, u64 n <= 1454,
 *            u128 n <= 5506, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035607: number of points of L1 norm m in Z^d, read by antidiagonals.
   The values f(d,m) satisfy
        f(d,0)     = 1               (d ≥ 1)
        f(d,d-1)   = 2               (d ≥ 2)
        f(d,m)     = f(d-1,m-1) + f(d-1,m) + f(d-2,m-1)   (d ≥ 3, 1 ≤ m ≤ d-2)
   with f(1,0)=1.  The index n is mapped to (d,m) by
        d = floor((sqrt(8n+1)-1)/2) + 1
        m = n - d(d-1)/2 .
   The sequence a(n) is f(d,m) for that pair. */

#include <stdlib.h>
#include <stdint.h>

/* Integer square‑root for 64‑bit unsigned values, without floating point. */
static uint64_t floor_sqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = UINT64_MAX, mid, ans = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Lazy cache of table values f(d,m).  dp[d] points to an array of size d (indices 0..d-1). */
static seq_t **dp = NULL;      /* dp[0] is unused (NULL) */
static size_t max_d = 0;       /* largest d currently allocated */

/* Ensure rows up to target_d are computed. */
static void ensure_dp_up_to(size_t target_d)
{
    if (target_d <= max_d) return;

    dp = (seq_t **)realloc(dp, (target_d + 1) * sizeof(seq_t *));
    if (!dp) abort();

    for (size_t d = max_d + 1; d <= target_d; ++d) {
        dp[d] = (seq_t *)calloc(d, sizeof(seq_t));
        if (!dp[d]) abort();

        dp[d][0] = (seq_t)1;          /* f(d,0) = 1 */
        if (d >= 2) dp[d][d - 1] = (seq_t)2;    /* f(d,d-1) = 2 */

        if (d >= 3) {
            for (size_t m = 1; m <= d - 2; ++m) {
                dp[d][m] = dp[d - 1][m - 1] + dp[d - 1][m] + dp[d - 2][m - 1];
            }
        }
    }
    max_d = target_d;
}

/* Convert linear index n into row d and column m. */
static void idx_to_dm(idx_t n, size_t *d_out, size_t *m_out)
{
    uint64_t s = floor_sqrt_u64(8 * (uint64_t)n + 1);
    size_t d = (s - 1) / 2 + 1;
    uint64_t base = (uint64_t)d * (d - 1) / 2;
    size_t m = (size_t)(n - base);
    *d_out = d;
    *m_out = m;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    size_t d, m;
    idx_to_dm(n, &d, &m);
    ensure_dp_up_to(d);
    return dp[d][m];
}
