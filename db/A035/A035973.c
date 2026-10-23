/*
 * A035973  Number of partitions of n into parts not of the form 19k, 19k+4 or
 *          19k-4. Also number of partitions with at most 3 parts of size 1
 *          and differences between parts at distance 8 are greater than 1.
 * https://oeis.org/A035973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not of the form 19k,
 *            19k+4 or 19k-4. The implementation uses a dynamic program that
 *            iterates over all allowed part sizes 1…n and builds up the
 *            partition counts in an array. All arithmetic is done in seq_t,
 *            the same integer type as used for the result
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate a temporary array of length n+1.
       The array is recreated on each call to keep the implementation
       simple and to comply with the requirement to work in any order. */
    static seq_t *dp = NULL;
    static idx_t  dp_size = 0;

    if (dp_size <= n) {
        /* Resize the array (deallocate previous one if any). */
        seq_t *new_dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_dp) abort();
        free(dp);
        dp = new_dp;
        dp_size = n + 1;
    }

    /* Initialise the DP array. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;   /* empty partition */

    /* Perform the classic coin‑change DP */
    for (idx_t p = 1; p <= n; ++p) {
        idx_t r = p % 19;
        if (r == 0 || r == 4 || r == 15)   /* forbidden part */
            continue;
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    return dp[n];
}
