/*
 * A035370  Number of partitions of n into parts 5k or 5k+4.
 * https://oeis.org/A035370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts that are 5k or 5k+4
 * Range:     u8 n <= 56, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a standard coin‑change DP.  No intermediate values
   overflow seq_t because every addition is performed on values that already
   fit in the type. */

#include <stddef.h>         /* for size_t */
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 */

    /* Allocate the DP array.  alloc_seq does not zero‑initialize, so do it. */
    seq_t *dp = alloc_seq((size_t)(n + 1));
    if (!dp) return 0;                   /* allocation failed */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Parts that are multiples of 5: 5,10,15,... */
    for (idx_t coin = 5; coin <= n; coin += 5) {
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    /* Parts that are 5k+4: 4,9,14,... */
    for (idx_t coin = 4; coin <= n; coin += 5) {
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free_seq(dp);
    return result;
}
