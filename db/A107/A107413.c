/*
 * A107413  a(n) = 7*a(n-1)-6*a(n-3)+a(n-5).
 * https://oeis.org/A107413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 6*a(n-3) + a(n-5) Initial values: a(0)=0,
 *            a(1)=1, a(2)=1, a(3)=2, a(4)=3 Implements recurrence with
 *            memoization and dynamic array growth
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp   = NULL;   /* cached terms */
    static idx_t dp_sz = 0;      /* number of terms currently stored */

    /* initialise the first five terms on first call */
    if (dp_sz == 0) {
        dp_sz = 5;
        dp = (seq_t*)calloc((size_t)dp_sz, sizeof(seq_t));
        if (!dp)            /* allocation failed */
            return (seq_t)0;
        dp[0] = (seq_t)0;
        dp[1] = (seq_t)1;
        dp[2] = (seq_t)1;
        dp[3] = (seq_t)2;
        dp[4] = (seq_t)3;
    }

    /* if the requested index is already computed, return it */
    if (n < dp_sz)
        return dp[n];

    /* grow the storage to accommodate up to n */
    size_t new_sz = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(dp, new_sz * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;      /* allocation failed */
    dp = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = dp_sz; i <= n; ++i) {
        dp[i] = 7 * dp[i-1] - 6 * dp[i-3] + dp[i-5];
    }
    dp_sz = new_sz;
    return dp[n];
}
