/*
 * A106435  a(n) = 3*a(n-1) + 3*a(n-2), a(0)=0, a(1)=3.
 * https://oeis.org/A106435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=3, a(n)=3*a(n-1)+3*a(n-2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *dp = NULL;
    static idx_t cap = 0;
    static idx_t last = 0;
    static int initialized = 0;

    if (!initialized) {
        /* Start with room for the first two terms */
        cap = 2;
        dp = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!dp) return (seq_t)0;          /* memory failure */
        dp[0] = (seq_t)0;
        dp[1] = (seq_t)3;
        last  = 1;
        initialized = 1;
    }

    if (n < cap)
        return dp[n];

    /* Grow the table to accommodate index n */
    idx_t new_cap = n + 1;
    seq_t *new_dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    if (!new_dp) return (seq_t)0;          /* memory failure */
    dp = new_dp;
    cap = new_cap;

    /* Compute missing values using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i)
        dp[i] = (seq_t)3 * dp[i - 1] + (seq_t)3 * dp[i - 2];

    last = n;
    return dp[n];
}
