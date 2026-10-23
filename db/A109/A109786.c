/*
 * A109786  Expansion of -(x+2*x^2+3*x^3-1+5*x^4)/((x+1)*(x^2-3*x+1)*(1+x^2)).
 * https://oeis.org/A109786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1)+a(n-2)+a(n-3)+2*a(n-4)-a(n-5),
 *            a(0)=a(1)=a(2)=a(3)=a(4)=1
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *dp = NULL;    /* memoized sequence values */
    static idx_t len = 0;       /* number of computed terms (dp[0..len-1]) */
    static idx_t cap = 0;       /* allocated capacity of dp */

    /* Initial allocation and first five terms */
    if (dp == NULL) {
        cap = 5;
        dp = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (dp == NULL) return (seq_t)0; /* allocation failure */
        dp[0] = dp[1] = dp[2] = dp[3] = dp[4] = (seq_t)1;
        len = 5;
    }

    /* Ensure storage large enough for requested index */
    if (n >= len) {
        while (cap <= n) {
            idx_t newcap = (cap == 0) ? 5 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(dp, newcap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* allocation failure */
            dp = tmp;
            cap = newcap;
        }

        /* Compute terms lazily up to n */
        for (idx_t i = len; i <= n; ++i) {
            seq_t a1 = dp[i - 1];
            seq_t a2 = dp[i - 2];
            seq_t a3 = dp[i - 3];
            seq_t a4 = dp[i - 4];
            seq_t a5 = dp[i - 5];

            /* 2*a1 + a2 + a3 + 2*a4 - a5
               performed using additions/subtractions only */
            seq_t val = a1 + a1;           /* 2*a1 */
            val += a2;                     /* + a2 */
            val += a3;                     /* + a3 */
            val += a4 + a4;                /* + 2*a4 */
            val -= a5;                     /* - a5 */
            dp[i] = val;
        }
        len = n + 1;
    }

    return dp[n];
}
