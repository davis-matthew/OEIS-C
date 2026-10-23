/*
 * A035948  Number of partitions of n into parts not of the form 11k, 11k+5 or
 *          11k-5. Also number of partitions with at most 4 parts of size 1
 *          and differences between parts at distance 4 are greater than 1.
 * https://oeis.org/A035948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 160, u64 n <= 539,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A035948 – partitions of n into parts not ≡ 0,5 or 6 (mod 11)  */
/*  (OEIS: a(n) = number of partitions of n avoiding parts 11k, 11k+5, 11k-5)  */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Allocate an array of size n+1 for dynamic programming.  */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;                /* allocation failure – should not happen for n ≤ 10000 */

    /* Initialise DP table.  */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i < len; ++i)
        dp[i] = (seq_t)0;

    /* Count partitions using allowed part sizes.  */
    for (idx_t s = 1; s <= n; ++s) {
        idx_t r = s % 11;
        if (r == 0 || r == 5 || r == 6)
            continue;                   /* forbidden part sizes */
        for (size_t j = (size_t)s; j < len; ++j) {
            dp[j] += dp[j - (size_t)s];
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
