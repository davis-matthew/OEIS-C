/*
 * A087750  Number of partitions of n into numbers of the form 2^i - 2^j with
 *          i > j.
 * https://oeis.org/A087750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into numbers of the form 2^i –
 *            2^j with i > j. (Generating function: ∏_{k>=1}∏_{j>=0} 1/(1 -
 *            x^{(2^k-1)*2^j}) )
 * Range:     u8 n <= 18, u16 n <= 56, u32 n <= 207, u64 n <= 971,
 *            u128 n <= 6034, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* dynamic dp array with static storage to avoid realloc per call
       when the same n is requested multiple times. */
    static seq_t *dp = NULL;
    static idx_t dp_cap = 0;

    /* ensure capacity */
    if (dp_cap < n + 1) {
        seq_t *tmp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – terminate (no output required). */
            exit(1);
        }
        dp = tmp;
        dp_cap = n + 1;
    }

    /* initialise dp[0..n] */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;                      /* empty partition */

    /* iterate over all parts v = (2^k - 1) * 2^j <= n   */
    for (idx_t j = 0; j < 64 && ((idx_t)1 << j) <= n; ++j) {
        for (idx_t k = 1; k < 64; ++k) {
            /* use 128‑bit arithmetic to avoid overflow of the shift */
            unsigned __int128 v128 = ((((unsigned __int128)1) << k) - 1) << j;
            if (v128 > n)
                break;
            idx_t v = (idx_t)v128;

            /* classic coin‑change DP (order‑independent) */
            for (idx_t i = v; i <= n; ++i) {
                dp[i] += dp[i - v];
            }
        }
    }

    return dp[n];
}
