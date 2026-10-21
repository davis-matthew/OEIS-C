/*
 * A219199  Number of partitions of n into 5 distinct primes.
 * https://oeis.org/A219199
 *
 * Offset:    28    (a(28) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to partition n into 5 distinct primes
 * Range:     u8 n <= 142, u16 n <= 712, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Count of partitions of n into five distinct primes. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 28.  Return 0 for all smaller n. */
    if (n < 28U) return 0;

    /* Static state: DP table and prime list. */
    static size_t dp_cap = 0;          /* maximum n for which DP is computed */
    static seq_t *dp = NULL;          /* DP table: dp[k][s] for k=0..5, s=0..dp_cap */
    static uint64_t *primes = NULL;   /* primes up to dp_cap */
    static size_t primes_cnt = 0;

    /* Allocate or resize DP table if needed. */
    if ((size_t)n > dp_cap) {
        /* Free previous resources. */
        free(dp);
        free(primes);

        /* Generate all primes <= n. */
        build_primes_upto(n, &primes, &primes_cnt);

        /* Allocate new DP table. */
        dp_cap = (size_t)n;
        dp = (seq_t *)malloc(6 * (dp_cap + 1) * sizeof(seq_t));
        if (!dp) return 0; /* allocation failure – should not happen */

        /* Initialise to zero and set base case. */
        for (size_t i = 0; i < 6 * (dp_cap + 1); ++i) dp[i] = 0;
        dp[0 * (dp_cap + 1) + 0] = 1;   /* one way to pick 0 primes summing to 0 */

        /* DP update: for each prime, add its contribution to the sums. */
        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            uint64_t p = primes[pi];
            if (p > (uint64_t)n) break;

            /* Update for k from 5 down to 1. */
            for (int k = 5; k >= 1; --k) {
                size_t base_k     = (size_t)k * (dp_cap + 1);
                size_t base_km1   = (size_t)(k - 1) * (dp_cap + 1);
                /* Sum variable as signed to use negative loop guard. */
                for (int s = (int)n; s >= (int)p; --s) {
                    dp[base_k + s] += dp[base_km1 + s - (int)p];
                }
            }
        }
    }

    /* After the table is ready, answer the query. */
    size_t idx = (size_t)n;
    return dp[5 * (dp_cap + 1) + idx];
}
