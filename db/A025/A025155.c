/*
 * A025155  Number of partitions of n into distinct parts >= 10.
 * https://oeis.org/A025155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts >= 10.
 *            Computation: dynamic programming over the infinite set
 *            {10,11,12,...}, stopping at n. dp[i] is the number of ways to
 *            write i as a sum of such distinct parts. The outer loop iterates
 *            over each possible part j from 10 up to the requested n...
 * Range:     u8 n <= 66, u16 n <= 137, u32 n <= 341, u64 n <= 981,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *dp = NULL;     /* dp[i] = number of partitions of i   */
    static idx_t      max_n = 0;  /* largest n for which dp is fresh     */

    /* if this is the first call or we need a larger table, recompute */
    if (dp == NULL || n > max_n) {
        /* allocate new array of size n+1 (indices 0..n) */
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) {
            abort();  /* out of memory */
        }
        dp = tmp;

        /* initialise all entries to 0, then set dp[0] = 1 */
        for (idx_t i = 0; i <= n; ++i) {
            dp[i] = (seq_t)0;
        }
        dp[0] = (seq_t)1;

        /* dynamic programming over parts >= 10 */
        for (idx_t j = 10; j <= n; ++j) {       /* part value      */
            for (idx_t i = n; i >= j; --i) {    /* sum target      */
                dp[i] += dp[i - j];
            }
        }

        max_n = n;
    }

    return dp[n];
}
