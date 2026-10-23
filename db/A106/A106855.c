/*
 * A106855  Expansion of 1/(1-x^2(1-3x)).
 * https://oeis.org/A106855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-2) - 3*a(n-3), a(0)=1, a(1)=0, a(2)=1
 * Range:     i8 n <= 11, i16 n <= 22, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the nth term of OEIS A106855 */
seq_t a(idx_t n)
{
    static seq_t *dp = NULL;      /* dynamically growing table of values   */
    static idx_t cap = 0;         /* number of elements currently stored   */

    /* Initialize base cases on first call */
    if (dp == NULL) {
        cap = 3;                  /* we need indices 0,1,2 */
        dp = (seq_t *)malloc(cap * sizeof(seq_t));
        if (dp == NULL) abort();  /* allocation failed */
        dp[0] = 1;
        dp[1] = 0;
        dp[2] = 1;
    }

    /* Grow table if needed */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *new_dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (new_dp == NULL) abort(); /* allocation failed */
        dp = new_dp;

        /* Compute missing terms using the recurrence */
        for (idx_t i = cap; i <= n; ++i) {
            dp[i] = dp[i - 2] - ((seq_t)3) * dp[i - 3];
        }
        cap = new_cap;
    }

    return dp[n];
}
