/*
 * A107299  a(n) = 4*a(n-1)-2*a(n-2)-3*a(n-3)+2*a(n-4), n>5.
 * https://oeis.org/A107299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)-2*a(n-2)-3*a(n-3)+2*a(n-4) initial values:
 *            a(0)=3, a(1)=3, a(2)=10, a(3)=29, a(4)=94, a(5)=292
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *dp      = NULL;   /* array of computed a(i) */
    static idx_t dp_size  = 0;      /* current allocated size (in elements) */
    static idx_t cached   = 0;      /* highest index that has been computed */

    /* always keep at least six slots for the initial values */
    idx_t need = n + 1;
    if (need < 6) need = 6;

    if (dp_size < need)
    {
        seq_t *tmp = (seq_t*)realloc(dp, (size_t)need * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;               /* allocation fail – should not happen */
        dp       = tmp;
        dp_size  = need;
    }

    /* initialise the first six terms only once */
    if (cached == 0)
    {
        dp[0] = (seq_t)3;
        dp[1] = (seq_t)3;
        dp[2] = (seq_t)10;
        dp[3] = (seq_t)29;
        dp[4] = (seq_t)94;
        dp[5] = (seq_t)292;
        cached = 5;
    }

    /* if the requested value is already cached, return it */
    if (n <= cached)
        return dp[n];

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = cached + 1; i <= n; ++i)
    {
        seq_t val = (seq_t)4 * dp[i-1]
                  - (seq_t)2 * dp[i-2]
                  - (seq_t)3 * dp[i-3]
                  + (seq_t)2 * dp[i-4];
        dp[i] = val;
    }
    cached = n;
    return dp[n];
}
