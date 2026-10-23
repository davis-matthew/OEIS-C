/*
 * A035454  Number of partitions of n into parts 8k+2 or 8k+3.
 * https://oeis.org/A035454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to 2 or 3
 *            modulo 8
 * Range:     u8 n <= 55, u16 n <= 149, u32 n <= 456, u64 n <= 1536,
 *            u128 n <= 5527, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP implementation: for each allowed coin c (2,3,10,11,…) add dp[j–c] to dp[j] (j ≥ c). */

#include <stdlib.h>
#ifdef __cplusplus
    #include <new>
#endif

seq_t a(idx_t n)
{
    /* trivial case: offset starts at 1, but handle n==0 safely */
    if (n == 0) return 0;

    /* allocate array of size n+1, with proper initialization for both C and C++ */
    seq_t* dp;
#ifdef __cplusplus
    dp = new seq_t[n + 1];
#else
    dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
#endif
    if (!dp) return 0;      /* allocation failed – unlikely for the given limits */

    /* initialise dp[0] = 1 (empty partition), rest are zero */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* iterate over all allowed part sizes up to n */
    for (idx_t coin = 2; coin <= n; ++coin) {
        if ((coin & 7) != 2 && (coin & 7) != 3)   /* coin ≡ 2 or 3 (mod 8) */
            continue;
        for (idx_t j = coin; j <= n; ++j) {
            dp[j] = dp[j] + dp[j - coin];
        }
    }

    seq_t result = dp[n];

    /* free allocated memory */
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}
