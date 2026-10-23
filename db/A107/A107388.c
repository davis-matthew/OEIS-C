/*
 * A107388  Expansion of x*(3*x-1)*(2*x-1) / ( (1-x)*(1+x)*(x^2-4*x+1) ).
 * https://oeis.org/A107388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 19, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=-1, a(3)=2,
   for n≥4: a(n)=4*a(n−1)−4*a(n−3)+a(n−4) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t maxComputed = 0;   /* highest index that has been filled */
    static idx_t dpCap = 0;        /* allocated capacity */

    /* Initial allocation and seed values */
    if (dpCap == 0) {
        dpCap = 4;                                      /* at least indices 0..3 */
        dp = (seq_t*)malloc(dpCap * sizeof(seq_t));
        if (!dp) return 0;           /* allocation failed */
        dp[0] = 0;
        dp[1] = 1;
        dp[2] = (seq_t)-1;
        dp[3] = 2;
        maxComputed = 3;
    }

    /* Grow the array if necessary */
    if (n >= dpCap) {
        idx_t newCap = n + 1;                 /* exact size sufficient */
        seq_t *tmp = (seq_t*)realloc(dp, newCap * sizeof(seq_t));
        if (!tmp) return 0;                   /* allocation failed */
        dp = tmp;
        dpCap = newCap;
    }

    /* Compute missing values up to n */
    if (n > maxComputed) {
        for (idx_t i = maxComputed + 1; i <= n; ++i) {
            /* recurrence applies for i ≥ 4; for i = 4..n uses previous values */
            dp[i] = 4*dp[i-1] - 4*dp[i-3] + dp[i-4];
        }
        maxComputed = n;
    }

    return dp[n];
}
