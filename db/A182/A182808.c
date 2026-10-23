/*
 * A182808  Number of partitions of 3n + 2 into parts >= 3.
 * https://oeis.org/A182808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of 3*n+2 into parts ≥ 3 (OEIS
 *            A182808)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 56, u64 n <= 168, u128 n <= 551,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;      /* DP array: dp[i] = partitions of i into parts ≥3 */
static size_t dp_size = 0;    /* allocated size */

/* Recompute dp up to index m (inclusive).  The algorithm uses only
   addition, thus works for any seq_t type. */
static void compute_dp_up_to(size_t m)
{
    if (dp_size > m) return;   /* already computed */

    /* reallocate to the new required size */
    dp = (seq_t *)realloc(dp, (m + 1) * sizeof(seq_t));
    dp_size = m + 1;

    /* initialise dp[0] = 1, dp[1..m] = 0 */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i <= m; ++i) dp[i] = (seq_t)0;

    /* coin‑change DP with part sizes starting from 3 */
    for (size_t part = 3; part <= m; ++part) {
        for (size_t sum = part; sum <= m; ++sum) {
            dp[sum] += dp[sum - part];
        }
    }
}

/* Public sequence routine.  Accepts any n ≥ 0, called in any order. */
seq_t a(idx_t n)
{
    /* target integer = 3*n + 2  (fits in size_t for the intended limits) */
    size_t m = (size_t)(n * ((idx_t)3) + 2);
    compute_dp_up_to(m);
    return dp[m];
}
