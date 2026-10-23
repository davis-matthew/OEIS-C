/*
 * A109360  Expansion of x*(1+4*x+5*x^2-x^3+6*x^4+x^5-4*x^6) /
 *          ((x^2+1)*(x^2-x+1)*(x-1)^2*(x+1)^2).
 * https://oeis.org/A109360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-3) + 2*a(n-4) - a(n-5) + a(n-7) - a(n-8) for
 *            n>7, with initial values a(0)=0, a(1)=1, a(2)=5, a(3)=10,
 *            a(4)=8, a(5)=11, a(6)=11, a(7)=14
 * Range:     u8 n <= 152, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static memoization array */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* initialise first 8 terms once */
    if (dp_size == 0) {
        dp_size = 8;
        dp = (seq_t *)malloc(dp_size * sizeof(seq_t));
        dp[0] = (seq_t)0;
        dp[1] = (seq_t)1;
        dp[2] = (seq_t)5;
        dp[3] = (seq_t)10;
        dp[4] = (seq_t)8;
        dp[5] = (seq_t)11;
        dp[6] = (seq_t)11;
        dp[7] = (seq_t)14;
    }

    /* if n already computed, return immediately */
    if (n < dp_size)
        return dp[n];

    /* extend the table up to n */
    /* ensure dp has enough capacity */
    if (dp_size <= n) {
        size_t new_size = n + 1;
        dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        /* compute terms from old size to n */
        for (idx_t i = dp_size; i <= n; ++i) {
            /* recurrence: a(n) = a(n-1) - a(n-3) + 2*a(n-4) - a(n-5) + a(n-7) - a(n-8) */
            seq_t val;
            val = dp[i-1] - dp[i-3];              /* a(n-1) - a(n-3) */
            val = val + dp[i-4] + dp[i-4];         /* + 2*a(n-4) */
            val = val - dp[i-5];                   /* - a(n-5) */
            val = val + dp[i-7];                   /* + a(n-7) */
            val = val - dp[i-8];                   /* - a(n-8) */
            dp[i] = val;
        }
        dp_size = new_size;
    }

    return dp[n];
}
