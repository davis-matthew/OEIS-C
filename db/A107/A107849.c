/*
 * A107849  Expansion of (1 + x)^2 / ((1 - x^2 - 2*x^3)*(1 + x^4)).
 * https://oeis.org/A107849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) satisfies *
 *            a(0)=1,a(1)=2,a(2)=2,a(3)=4,a(4)=5,a(5)=6,a(6)=12 *
 *            a(n)=a(n-2)+2*a(n-3)-a(n-4)+a(n-6)+2*a(n-7) for n>6
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 53, u64 n <= 105,
 *            u128 n <= 211, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization array */
    static seq_t *dp = NULL;
    static idx_t dp_sz = 0;     /* number of initialized elements */
    static idx_t dp_cap = 0;    /* allocated capacity */

    /* ensure capacity for at least n+1 elements */
    if (n + 1 > dp_cap) {
        idx_t new_cap = dp_cap ? dp_cap * 2 : 8;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *new_dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!new_dp) return 0; /* allocation failure, return 0 as safe default */
        dp = new_dp;
        dp_cap = new_cap;
    }

    /* initialize base values if not yet done */
    if (dp_sz == 0) {
        dp[0] = (seq_t)1;
        dp[1] = (seq_t)2;
        dp[2] = (seq_t)2;
        dp[3] = (seq_t)4;
        dp[4] = (seq_t)5;
        dp[5] = (seq_t)6;
        dp[6] = (seq_t)12;
        dp_sz = 7;
    }

    /* compute missing values up to n */
    for (idx_t i = dp_sz; i <= n; ++i) {
        seq_t t1 = dp[i - 2];
        seq_t t2 = dp[i - 3];
        seq_t t3 = dp[i - 4];
        seq_t t4 = dp[i - 6];
        seq_t t5 = dp[i - 7];
        dp[i] = t1 + (t2 << 1) - t3 + t4 + (t5 << 1);
    }

    if (n + 1 > dp_sz) dp_sz = n + 1;

    return dp[n];
}
