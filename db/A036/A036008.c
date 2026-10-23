/*
 * A036008  Number of partitions of n into parts not of the form 25k, 25k+9 or
 *          25k-9. Also number of partitions with at most 8 parts of size 1
 *          and differences between parts at distance 11 are greater than 1.
 * https://oeis.org/A036008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not congruent to 0,
 *            9, or 16 modulo 25. Computed by the classic 1‑dimensional DP
 *            algorithm for partition numbers with forbidden part sizes
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 135, u64 n <= 451,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate DP table for sums 0 … n. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* allocation failure: safe fallback */

    dp[0] = (seq_t)1;                  /* empty partition of 0 */
    /* All other slots are initially zero (malloc returns indeterminate
       values; explicitly set them). */
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Iterate over all possible part sizes. */
    for (idx_t part = 1; part <= n; ++part) {
        idx_t mod = part % (idx_t)25;
        if (mod == 0 || mod == 9 || mod == 16) continue;  /* forbidden */

        for (idx_t sum = part; sum <= n; ++sum) {
            dp[sum] += dp[sum - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
