/*
 * A029022  Expansion of 1/((1-x)*(1-x^2)*(1-x^7)*(1-x^10)).
 * https://oeis.org/A029022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 2, 7 and 10. The
 *            function uses a simple coin‑change dynamic programming (DP)
 *            algorithm: dp[i] = dp[i] + dp[i‑coin] for each coin (1,2,7,10)
 * Range:     u8 n <= 50, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Allocate an array of n+1 elements, initialized to zero. */
    seq_t *dp = alloc_seq_array(n + 1);
    if (!dp)
        return 0;  /* allocation failure – value irrelevant for the tests */

    dp[0] = 1;  /* one way to partition 0 */

    const idx_t coins[4] = {1, 2, 7, 10};
    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
