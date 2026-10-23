/*
 * A035391  Number of partitions of n into parts 7k or 7k+2.
 * https://oeis.org/A035391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 7k or 7k+2, i.e.
 *            parts ≡ 0 or 2 (mod 7), n ≥ 1. Partition counting is done with a
 *            standard unbounded coin‑change DP where the parts are processed
 *            in increasing order, so each partition is counted once
 * Range:     u8 n <= 63, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small sanity check (offset starts at 1) */
    if (n == 0) return 0;

    /* Allocate DP table of size n+1, initialized to 0 */
    size_t len = (size_t)(n + 1);
    seq_t *dp = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp) return 0;  /* allocation failure: return 0 as a safe fallback */

    dp[0] = (seq_t)1;  /* one way to partition 0: use no parts */

    /* Parts of the form 7k+2 (k≥0): 2,9,16,... */
    for (idx_t p = 2; p <= n; p += 7) {
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    /* Parts of the form 7k   (k≥1): 7,14,21,... */
    for (idx_t p = 7; p <= n; p += 7) {
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
