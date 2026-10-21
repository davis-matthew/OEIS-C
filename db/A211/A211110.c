/*
 * A211110  Number of partitions of n into divisors > 1 of n.
 * https://oeis.org/A211110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into divisors > 1 of n
 * Range:     u8 n <= 35, u16 n <= 119, u32 n <= 335, u64 n <= 1079,
 *            u128 n <= 5039, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* External types idx_t and seq_t are defined by the build system */

seq_t a(idx_t n)
{
    /* Handle the special cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* ---------------------------------------------
       1. Gather all divisors d > 1 of n (including n itself).
       --------------------------------------------- */
    /* The maximum number of divisors is small; allocating
       (n + 1) entries is safe for n up to a few thousand.      */
    idx_t *parts = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!parts) return (seq_t)0;        /* allocation failure */

    size_t part_cnt = 0;

    /* n itself is always a divisor ≥ 2 */
    parts[part_cnt++] = n;

    for (idx_t d = 2; d <= n / d; ++d) {   /* iterate up to sqrt(n) */
        if (n % d == 0) {
            parts[part_cnt++] = d;
            idx_t other = n / d;
            if (other != d) {
                parts[part_cnt++] = other;
            }
        }
    }

    /* ---------------------------------------------
       2. Unbounded‑knapsack DP: count partitions using
          the collected parts.
       --------------------------------------------- */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) {
        free(parts);
        return (seq_t)0;                /* allocation failure */
    }
    memset(dp, 0, (size_t)(n + 1) * sizeof(seq_t));
    dp[0] = (seq_t)1;                   /* empty partition */

    for (size_t k = 0; k < part_cnt; ++k) {
        idx_t part = parts[k];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    /* The answer for n is stored at dp[n] */
    seq_t result = dp[n];

    /* Clean up allocated memory */
    free(parts);
    free(dp);

    return result;
}
