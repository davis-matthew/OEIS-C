/*
 * A179051  Number of partitions of n into powers of 10 (cf. A011557).
 * https://oeis.org/A179051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into powers of 10. Computed by
 *            dynamic programming
 * Range:     u8 n <= 659, u16 n <= 6779, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* dp keeps the number of partitions for all values up to the
       current capacity. It grows only when a larger n is requested. */
    static seq_t *dp = NULL;
    static idx_t capacity = 0;         /* number of slots currently allocated */

    if (n >= capacity) {
        /* Allocate a new array large enough for all values up to n. */
        seq_t *new_dp = alloc_seq_array(n + 1);
        if (!new_dp) {                 /* allocation failed */
            free_seq_array(dp);
            dp = NULL;
            capacity = 0;
            return 0;
        }

        /* Preserve any previously computed values */
        for (idx_t i = 0; i < capacity; ++i)
            new_dp[i] = dp[i];

        /* Replace the old array */
        free_seq_array(dp);
        dp = new_dp;
        capacity = n + 1;

        /* Re‑compute the whole table up to n. */
        for (idx_t i = 0; i <= n; ++i)
            dp[i] = 0;
        dp[0] = 1; /* one way to reach 0: use no parts */

        /* Iterate over coin denominations: 1, 10, 100, … */
        for (idx_t coin = 1; coin <= n; ) {
            for (idx_t i = coin; i <= n; ++i)
                dp[i] += dp[i - coin];

            /* Prevent overflow when multiplying by 10. */
            if (coin > n / 10)
                break;
            coin *= 10;
        }
    }

    return dp[n];
}
