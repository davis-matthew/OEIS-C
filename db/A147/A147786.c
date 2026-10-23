/*
 * A147786  Number of partitions of n into parts divisible by 4 or 5.
 * https://oeis.org/A147786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Partitions of n into parts divisible by 4 or 5. Computed by a
 *            standard coin‑change DP: dp[0] = 1 dp[i] += dp[i‑p] for every
 *            part p = 4k or 5k (k ≥ 1)
 * Range:     u8 n <= 55, u16 n <= 127, u32 n <= 352, u64 n <= 1098,
 *            u128 n <= 3750, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Allocate an array large enough for dp[0 .. n] */
#if defined(__cplusplus)
    seq_t *dp = new seq_t[(size_t)(n + 1)]();   /* value‑initialised to zero */
#else
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) {     /* handle OOM – abort out of band */
        exit(1);
    }
    /* initialise the table to zero */
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = (seq_t)0;
    }
#endif

    dp[0] = (seq_t)1;

    /* Parts that are multiples of 4 */
    for (idx_t part = 4; part <= n; part += 4) {
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    /* Parts that are multiples of 5 but not of 20 (already counted) */
    for (idx_t part = 5; part <= n; part += 5) {
        if (part % 20 == 0) continue;
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];

#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}
