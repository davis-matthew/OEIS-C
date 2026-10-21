/*
 * A088954  G.f.: 1/((1-x)^2*(1-x^2)*(1-x^4)*(1-x^8)*(1-x^16)).
 * https://oeis.org/A088954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 80, u32 n <= 864, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of partitions of 2*n into powers of 2 that do 
 * not exceed 2^5 (i.e., 1,2,4,8,16,32).  
 * The function recomputes the partition numbers via a standard unbounded
 * coin‑change DP every time it is called; this guarantees correctness 
 * without complex memoisation.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* the coin values (powers of two up to 32) */
    static const idx_t coins[] = {1, 2, 4, 8, 16, 32};
    const size_t coin_cnt = 6;

    /* amount to partition: 2*n */
    idx_t max = 2 * n;
    size_t len = (size_t)(max + 1);

    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) abort();                     /* Out of memory */

    /* initialise dp[0] = 1, others 0 */
    dp[0] = 1;
    for (size_t i = 1; i < len; ++i)
        dp[i] = 0;

    /* unbounded coin change DP */
    for (size_t ci = 0; ci < coin_cnt; ++ci) {
        idx_t coin = coins[ci];
        /* if coin > max, this coin cannot be used */
        if (coin > max) continue;
        for (idx_t amt = coin; amt <= max; ++amt) {
            dp[amt] += dp[amt - coin];
        }
    }

    seq_t result = dp[max];
    free(dp);
    return result;
}
