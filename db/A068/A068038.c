/*
 * A068038  Number of subsets of {1,2,3,...,n} that sum to 0 mod 17.
 * https://oeis.org/A068038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   Computes the number of subsets of {1,2,…,n} whose sum is
   congruent to 0 modulo 17.
   Uses a linear DP over the residues mod 17, caching intermediate
   results so that multiple calls in any order are efficient.
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t dp[17];
    static seq_t *cache = NULL;
    static idx_t cache_len = 0U;

    /* initialise state if first call */
    if (cache_len == 0U)
    {
        for (int r = 0; r < 17; ++r) dp[r] = 0;
        dp[0] = 1;
        cache_len = 1U;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* memory failure – return 0 */
        cache[0] = dp[0];
    }

    /* extend cache to n, updating dp */
    while (cache_len <= n)
    {
        /* compute dp for next i = cache_len */
        idx_t i = cache_len;
        int imod = (int)(i % 17U);
        seq_t newdp[17];
        /* start with old dp */
        for (int r = 0; r < 17; ++r) newdp[r] = dp[r];
        /* add contributions of subsets that include i */
        for (int r = 0; r < 17; ++r)
            newdp[(r + imod) % 17] += dp[r];

        /* copy back to dp */
        for (int r = 0; r < 17; ++r) dp[r] = newdp[r];

        /* extend cache */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (cache_len + 1));
        if (!tmp) return 0;          /* memory failure – return 0 */
        cache = tmp;
        cache[cache_len] = dp[0];
        ++cache_len;
    }

    return cache[n];
}
