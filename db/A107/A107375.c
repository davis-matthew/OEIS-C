/*
 * A107375  a(n) = 6*a(n-1)-6*a(n-3)-a(n-4).
 * https://oeis.org/A107375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-3) - a(n-4) Initial values: a(0)=0,
 *            a(1)=1, a(2)=1, a(3)=2, a(4)=6
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;        /* memoization array */
static idx_t max_n = 0;         /* highest index computed so far */
static size_t cap = 0;          /* allocated capacity */

/* Ensure the memoization array contains all values up to n */
static void ensure(idx_t n)
{
    if (dp == NULL) {
        /* allocate for at least 5 values (0..4) */
        cap = (size_t)((n < 5) ? 5 : n + 1);
        dp = (seq_t *)malloc(cap * sizeof(seq_t));
        /* assign initial known terms */
        dp[0] = (seq_t)0;
        dp[1] = (seq_t)1;
        dp[2] = (seq_t)1;
        dp[3] = (seq_t)2;
        dp[4] = (seq_t)6;
        max_n = 4;
    } else if ((idx_t)cap <= n) {
        /* increase capacity (double strategy) */
        size_t new_cap = cap * 2;
        if ((size_t)n + 1 > new_cap)
            new_cap = (size_t)n + 1;
        dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute any missing values up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        dp[i] = (seq_t)(6 * dp[i-1] - 6 * dp[i-3] - dp[i-4]);
    }
    max_n = (idx_t)cap <= n ? n : max_n;  /* keep track of last computed */
}

seq_t a(idx_t n)
{
    ensure(n);
    return dp[n];
}
