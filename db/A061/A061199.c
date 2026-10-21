/*
 * A061199  Upper right triangle read by columns where T(n,k), with k >= n, is
 *          the number of partitions of k where no part appears more than n
 *          times; also partitions of k where no parts are multiples of (n+1).
 * https://oeis.org/A061199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 159, u16 n <= 1000, u32 n <= 8293, u64 n <= 9032,
 *            u128 n <= 9032, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of partitions of k with no part divisible by (r+1),
 *  where (k,r) are derived from n by writing n in column‑reading order
 *  of the upper-right triangle. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer square root for unsigned 64‑bit values */
static idx_t int_sqrt(idx_t v) {
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid != 0 && mid > v / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Compute T(r,k) = partitions of k with no part divisible by (r+1) */
static seq_t compute_T(idx_t r, idx_t k) {
    /* small k (≤ about 2000 for requested n); allocate on heap */
    seq_t *dp = (seq_t *)malloc((k + 1) * sizeof(seq_t));
    if (!dp) { /* out‑of‑memory: return 0 as a safe fallback */
        return 0;
    }
    dp[0] = 1;
    for (idx_t i = 1; i <= k; ++i)
        dp[i] = 0;

    idx_t mod = r + 1;
    for (idx_t coin = 1; coin <= k; ++coin) {
        if (coin % mod == 0)
            continue;
        for (idx_t s = coin; s <= k; ++s)
            dp[s] = dp[s] + dp[s - coin];
    }
    seq_t res = dp[k];
    free(dp);
    return res;
}

/* Public entry point: returns a(n) – the n‑th term of A061199. */
seq_t a(idx_t n) {
    /* Determine column k and row r from index n */
    idx_t v = 8 * n + 1;
    idx_t s = int_sqrt(v);
    idx_t k = (s - 1) / 2;                     /* k such that k(k+1)/2 ≤ n < (k+1)(k+2)/2 */
    idx_t before = k * (k + 1) / 2;
    idx_t r = n - before;                       /* 0 ≤ r ≤ k */
    return compute_T(r, k);
}
