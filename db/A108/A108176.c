/*
 * A108176  a(1) = 1, a(n) = (Sum_{k=1..floor(n/2)} 1/a(n + 1 -
 *          2k))*(Product_{k=1..floor(n/2)} a(n + 1 - 2k)).
 * https://oeis.org/A108176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+4) = a(n+1)*(a(n+2)-a(n)*a(n+1)) + a(n+2)*a(n+3) with
 *            initial values a(1)=1, a(2)=1, a(3)=1, a(4)=2, a(5)=3
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 11, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t cap = 0;
    static idx_t len = 0;

    /* initialise first five terms on first call */
    if (len == 0) {
        cap = 8;
        dp = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!dp) return (seq_t)0;          /* allocation failure */
        dp[0] = 1;                          /* a(1) */
        dp[1] = 1;                          /* a(2) */
        dp[2] = 1;                          /* a(3) */
        dp[3] = 2;                          /* a(4) */
        dp[4] = 3;                          /* a(5) */
        len  = 5;
    }

    if (n <= len)        /* already computed */
        return dp[n - 1];

    /* extend allocation if necessary */
    if (n > cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 8;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        dp = tmp;
        cap = (idx_t)new_cap;
    }

    /* compute new terms up to n using the recurrence */
    for (idx_t i = len + 1; i <= n; ++i) {
        if (i == 5) continue;               /* already present */
        idx_t idx = i - 1;                   /* 0‑based index of a(i) */

        /* a(i) = a(i-3)*(a(i-2) - a(i-4)*a(i-3)) + a(i-2)*a(i-1) */
        seq_t t0 = dp[idx - 4];               /* a(i-4) */
        seq_t t1 = dp[idx - 3];               /* a(i-3) */
        seq_t t2 = dp[idx - 2];               /* a(i-2) */
        seq_t t3 = dp[idx - 1];               /* a(i-1) */

        seq_t prod1 = t0 * t1;                /* a(i-4)*a(i-3) */
        seq_t diff   = t2 - prod1;           /* a(i-2) - a(i-4)*a(i-3) */
        seq_t term1  = t1 * diff;             /* a(i-3)*(…)      */
        seq_t term2  = t2 * t3;               /* a(i-2)*a(i-1)   */
        dp[idx] = term1 + term2;
    }
    len = n;
    return dp[n - 1];
}
