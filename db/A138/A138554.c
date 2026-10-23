/*
 * A138554  Minimal value of sum k_i when sum (k_i)^2 = n.
 * https://oeis.org/A138554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = minimal sum of k_i such that Σ (k_i)² = n (OEIS A138554)
 * Range:     u8 n <= 294, u16 n <= 9521, u32 n <= 9521, u64 n <= 9521,
 *            u128 n <= 9521, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root: floor(√n) for unsigned 64‑bit values */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;           /* 2^32 > √(2^64-1) */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid != 0 && n / mid < mid) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *dp = NULL;
    static idx_t dp_current = 0;   /* highest n for which dp[n] is valid */

    /* Allocate cache on first call */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (dp == NULL) return 0;          /* allocation failed */
        dp[0] = 0;
        dp_current = 0;
    }

    /* Extend cache if needed */
    if (dp_current < n) {
        seq_t *tmp = (seq_t *)realloc(dp, sizeof(seq_t) * (n + 1));
        if (tmp == NULL) return 0;         /* allocation failed */
        dp = tmp;

        for (idx_t i = dp_current + 1; i <= n; ++i) {
            /* Upper bound: all ones */
            seq_t best = (seq_t)i;

            idx_t maxk = isqrt_u64(i);
            for (idx_t k = 1; k <= maxk; ++k) {
                seq_t cand = dp[i - k * k] + (seq_t)k;
                if (cand < best) best = cand;
            }
            dp[i] = best;
        }
        dp_current = n;
    }

    return dp[n];
}
