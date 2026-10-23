/*
 * A015745  Number of partitions of n into distinct parts, none being 3.
 * https://oeis.org/A015745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 84, u32 n <= 248, u64 n <= 812,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: count partitions of n into distinct parts, none being 3
   using a 0‑1 knapsack DP.  For n = 0 the value is 1. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate a table of size n+1; dp[i] will hold the number of
       partitions of i with distinct parts excluding 3. */
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;          /* allocation failure: return 0 as fallback */

    /* initialise */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;                  /* one way to partition 0 (empty partition) */

    /* iterate over each allowed part size k */
    for (idx_t k = 1; k <= n; ++k) {
        if (k == 3) continue;   /* part 3 is forbidden */
        for (idx_t i = n; i >= k; --i) {
            dp[i] += dp[i - k]; /* include k once for each partial partition */
        }
    }

    /* result is stored at dp[n] */
    seq_t result = dp[n];
    free(dp);
    return result;
}
