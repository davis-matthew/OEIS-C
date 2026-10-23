/*
 * A106568  Expansion of 4*x/(1 - 4*x - 4*x^2).
 * https://oeis.org/A106568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=4, a(n)=4*a(n-1)+4*a(n-2) for n>1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;   /* memoisation table */
    static idx_t max_n = 0;    /* largest index already computed */

    /* initialise the table on first call */
    if (dp == NULL) {
        dp = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!dp) return (seq_t)0;          /* allocation failure */
        dp[0] = (seq_t)0;
        dp[1] = (seq_t)4;
        max_n = 1;
    }

    if (n == 0) return dp[0];
    if (n <= max_n) return dp[n];

    /* grow the table to accommodate the requested index */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(dp, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* allocation failure */
    dp = tmp;

    /* compute remaining terms using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        dp[i] = dp[i - 1] * ((seq_t)4) + dp[i - 2] * ((seq_t)4);
    }
    max_n = n;
    return dp[n];
}
