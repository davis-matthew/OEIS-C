/*
 * A107298  a(n) = 3*a(n-1)-a(n-2)-2*a(n-3)+a(n-4), n>5.
 * https://oeis.org/A107298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) + a(n-4) (n > 5) Initial
 *            values: a(0)=3, a(1)=2, a(2)=5, a(3)=10, a(4)=25, a(5)=56
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *dp = NULL;
    static idx_t  sz = 0;            /* number of elements stored (dp[0] … dp[sz-1]) */

    /* initialise base values on first call */
    if (sz == 0) {
        dp = (seq_t*)malloc(6 * sizeof(seq_t));
        dp[0] = (seq_t)3;
        dp[1] = (seq_t)2;
        dp[2] = (seq_t)5;
        dp[3] = (seq_t)10;
        dp[4] = (seq_t)25;
        dp[5] = (seq_t)56;
        sz = 6;
    }

    /* return directly if the value is already known */
    if (n < sz) {
        return dp[n];
    }

    /* resize the array to hold values up to index n */
    seq_t *tmp = (seq_t*)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
    if (tmp == NULL) { /* allocation failure – fallback to reallocate the same size */
        tmp = dp;
    }
    dp = tmp;            /* tmp is non‑NULL for valid sizes */

    /* compute missing terms using the recurrence */
    for (idx_t i = sz; i <= n; ++i) {
        dp[i] = dp[i-1] * (seq_t)3 - dp[i-2] - dp[i-3] * (seq_t)2 + dp[i-4];
    }

    sz = n + 1;
    return dp[n];
}
