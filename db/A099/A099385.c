/*
 * A099385  Number of partitions of 2*n into minimal numbers.
 * https://oeis.org/A099385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   _divisor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes number of partitions of 2*n into minimal numbers (OEIS A099385). */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/_divisor_count.h"

seq_t a(idx_t n)
{
    /* Target sum for the partition problem */
    idx_t target = 2 * n;

    /* ------------------------------------------------------------------ */
    /* Build the list of minimal numbers up to the target value.          */
    /* ------------------------------------------------------------------ */
    /* min_for_d[d] = minimal number with exactly d divisors, or 0 if not yet found. */
    idx_t *min_for_d = (idx_t *)calloc((size_t)(target + 1), sizeof(idx_t));
    if (!min_for_d) return 0;   /* OOM, return zero */

    idx_t x;
    for (x = 1; x <= target; ++x) {
        idx_t d = _divisor_count(x);
        if (d <= target && min_for_d[d] == 0) {
            min_for_d[d] = x;
        }
    }

    /* Count the number of distinct minimal numbers (coins). */
    size_t coins_len = 0;
    for (x = 1; x <= target; ++x) {
        if (min_for_d[x] != 0) {
            ++coins_len;
        }
    }

    /* Allocate array for the coins. */
    idx_t *coins = (idx_t *)malloc(coins_len * sizeof(idx_t));
    if (!coins) {
        free(min_for_d);
        return 0;   /* OOM, return zero */
    }

    /* Fill the coins array. */
    size_t idx = 0;
    for (x = 1; x <= target; ++x) {
        if (min_for_d[x] != 0) {
            coins[idx++] = min_for_d[x];
        }
    }

    /* We no longer need min_for_d. */
    free(min_for_d);

    /* ------------------------------------------------------------------ */
    /* Coin‑change DP: dp[s] = number of partitions of s using the coins. */
    /* ------------------------------------------------------------------ */
    seq_t *dp = (seq_t *)calloc((size_t)(target + 1), sizeof(seq_t));
    if (!dp) {
        free(coins);
        return 0;   /* OOM, return zero */
    }
    dp[0] = 1;   /* empty partition */

    size_t ci;
    for (ci = 0; ci < coins_len; ++ci) {
        idx_t coin = coins[ci];
        idx_t s;
        for (s = coin; s <= target; ++s) {
            dp[s] += dp[s - coin];
        }
    }

    seq_t result = dp[target];

    free(dp);
    free(coins);
    return result;
}
