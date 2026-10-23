/*
 * A026808  a(n) is the number of products P of distinct positive integers
 *          satisfying P <= n.
 * https://oeis.org/A026808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of products of distinct positive integers ≤ n *
 *            using at least two integers, with product ≤ n
 * Range:     u8 n <= 68, u16 n <= 4351, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Allocate array dp[1..n] to count subsets WITHOUT 1.
     * dp[p] = number of subsets of {2..n} whose product equals p. */
#ifdef __cplusplus
    seq_t *dp = new seq_t[(size_t)(n + 1)]();
#else
    seq_t *dp = calloc((size_t)(n + 1), sizeof(seq_t));
#endif
    if (dp == NULL)
        return (seq_t)0;

    dp[1] = 1;  /* empty subset for numbers >1 */

    for (idx_t i = 2; i <= n; ++i)
    {
        idx_t maxp = n / i;
        for (idx_t p = maxp; p >= 1; --p)
        {
            dp[(size_t)(p * i)] += dp[(size_t)p];
        }
    }

    seq_t sum = (seq_t)0;
    for (idx_t p = 1; p <= n; ++p)
        sum += dp[(size_t)p];

    /* Include subsets containing 1 (doubling).  Subtract
     *  n+1 subsets of size 0 or 1 to leave only size ≥ 2. */
    seq_t result = (seq_t)2 * sum - ((seq_t)n + 1);

#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
