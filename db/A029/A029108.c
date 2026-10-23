/*
 * A029108  Expansion of g.f. 1/((1-x)*(1-x^6)*(1-x^7)*(1-x^10)).
 * https://oeis.org/A029108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029108: number of partitions of n into parts 1, 6, 7, 10. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate a temporary DP array of size n+1. */
    seq_t *dp = (seq_t *)malloc((n + 1U) * sizeof(seq_t));
    if (!dp)
        abort();                     /* allocation failed – no recovery possible */

    dp[0] = (seq_t)1;                /* base case: one way to form 0 */
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;            /* initialise remaining entries */

    /* Coin 1 */
    for (idx_t i = 1; i <= n; ++i)
        dp[i] += dp[i - 1];

    /* Coin 6 */
    for (idx_t i = 6; i <= n; ++i)
        dp[i] += dp[i - 6];

    /* Coin 7 */
    for (idx_t i = 7; i <= n; ++i)
        dp[i] += dp[i - 7];

    /* Coin 10 */
    for (idx_t i = 10; i <= n; ++i)
        dp[i] += dp[i - 10];

    seq_t result = dp[n];
    free(dp);
    return result;
}
