/*
 * A092885  Number of partitions of n in which no parts are multiples of 25.
 * https://oeis.org/A092885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n in which no part is a multiple
 *            of 25
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 128, u64 n <= 421,
 *            u128 n <= 1487, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the generating function Π_{k>0} (1 - x^(25k))/(1 - x^k) implemented */
/* via a classic unbounded knapsack DP counting partitions with forbidden part sizes. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;   /* dp[i] = a(i) */
    static idx_t cap = 0;      /* current size of dp (max index + 1) */

    if (n >= cap) {                    /* need to grow and recompute */
        idx_t new_cap = n + 1;          /* array size for indices 0..n */
        seq_t *new_dp = (seq_t*)malloc(sizeof(seq_t) * new_cap);
        if (!new_dp) return (seq_t)0;   /* allocation failure, return 0 */

        /* initialise dp[0] = 1 and others to 0 */
        for (idx_t i = 0; i < new_cap; ++i) {
            new_dp[i] = (i == 0) ? (seq_t)1 : (seq_t)0;
        }

        /* unbounded knapsack: outer loop over allowed part sizes */
        for (idx_t part = 1; part < new_cap; ++part) {
            if (part % 25 == 0) continue;   /* skip multiples of 25 */
            for (idx_t sum = part; sum < new_cap; ++sum) {
                new_dp[sum] += new_dp[sum - part];
            }
        }

        /* replace old array */
        if (dp) free(dp);
        dp = new_dp;
        cap = new_cap;
    }

    return dp[n];
}
