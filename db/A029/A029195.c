/*
 * A029195  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^6)*(1-x^9)).
 * https://oeis.org/A029195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 585, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029195: Number of partitions of n into parts 2,5,6,9.
   Computation via dynamic programming:
      dp[0] = 1;
      for each coin in {2,5,6,9}
          for i = coin .. n
              dp[i] += dp[i - coin];
   The array `dp` is stored statically and recomputed only when a larger
   index is requested. */
#include <stdlib.h>

static seq_t *dp = NULL;     /* stored counts for 0..max_n */
static idx_t max_n = 0;      /* largest index computed in dp */

static const int coins[4] = {2, 5, 6, 9};

/* Return the number of partitions of n into parts 2,5,6,9. */
seq_t a(idx_t n)
{
    /* Allocate and compute dp up to `n` if needed. */
    if (dp == NULL || n > max_n) {
        size_t sz = (size_t)(n + 1);
        seq_t *new_dp = (seq_t *)calloc(sz, sizeof(seq_t));
        if (new_dp == NULL)
            return 0;   /* allocation failed */

        new_dp[0] = 1;   /* base case */

        /* Dynamic‑programming loops (outer: coins, inner: sums). */
        for (int k = 0; k < 4; ++k) {
            int c = coins[k];
            for (idx_t i = (idx_t)c; i <= n; ++i) {
                new_dp[(size_t)i] += new_dp[(size_t)(i - (idx_t)c)];
            }
        }

        /* Replace old table, free when appropriate. */
        free(dp);
        dp = new_dp;
        max_n = n;
    }

    return dp[(size_t)n];
}
