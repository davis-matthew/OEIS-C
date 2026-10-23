/*
 * A107368  a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-5).
 * https://oeis.org/A107368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 66, u64 n <= 127,
 *            u128 n <= 248, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) – a(n-3) + a(n-5)  
   Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=0, a(4)=1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for memoization */
    static seq_t *dp = NULL;          /* allocated array of terms   */
    static idx_t dp_cap = 0;          /* allocated length           */
    static idx_t last_computed = 0;   /* highest index that has been
                                         computed and stored      */

    /* First allocation: allocate enough space for 'n' plus the
       initial 5 terms, and initialise them. */
    if (dp_cap == 0) {
        idx_t new_cap = n + 1;
        if (new_cap < 5) new_cap = 5;
        dp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!dp) abort();
        dp[0] = 0;   /* a(0) */
        dp[1] = 0;   /* a(1) */
        dp[2] = 1;   /* a(2) */
        dp[3] = 0;   /* a(3) */
        dp[4] = 1;   /* a(4) */
        dp_cap = new_cap;
        last_computed = 4;    /* we have computed up to a(4) */
    }
    /* Expand storage if required for indices up to 'n' */
    else if (dp_cap <= n) {
        idx_t new_cap = dp_cap * 2;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        dp = tmp;
        dp_cap = new_cap;
    }

    /* Compute any missing terms up to 'n' */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t v1 = dp[i - 1];
            seq_t v2 = dp[i - 2];
            seq_t v3 = dp[i - 3];
            seq_t v5 = dp[i - 5];
            dp[i] = v1 + v2 - v3 + v5;
        }
        last_computed = n;
    }
    return dp[n];
}
