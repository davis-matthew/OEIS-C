/*
 * A029220  Expansion of 1/((1-x^2)*(1-x^6)*(1-x^8)*(1-x^11)).
 * https://oeis.org/A029220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into the parts 2, 6, 8, and 11.
 *            Computed by the classic coin‑change dynamic programming
 *            algorithm
 * Range:     u8 n <= 99, u16 n <= 727, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Allocate an array large enough to hold a(0)...a(n). */
    seq_t *dp = alloc_seq_array((size_t)(n + 1));
    if (!dp) abort();                /* out of memory – fail fast */

    /* initialise: only n=0 has one partition (empty sum).  */
    dp[0] = (seq_t)1;

    /* denominations that are allowed in the partition. */
    const idx_t coins[] = {2, 6, 8, 11};
    const size_t num_coins = sizeof(coins) / sizeof(coins[0]);

    for (size_t ci = 0; ci < num_coins; ++ci) {
        idx_t coin = coins[ci];
        if (coin > n) continue;
        for (idx_t k = coin; k <= n; ++k) {
            dp[(size_t)k] += dp[(size_t)(k - coin)];
        }
    }

    seq_t result = dp[(size_t)n];
    free_seq_array(dp);
    return result;
}
