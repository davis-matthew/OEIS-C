/*
 * A106603  a(n) = - 2*a(n-1) - 8*a(n-3), a(0) = 1, a(1) = 1, a(2) = -2.
 * https://oeis.org/A106603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-1) - 8*a(n-3), a(0)=1, a(1)=1, a(2)=-2
 * Range:     i8 n <= 8, i16 n <= 13, i32 n <= 23, i64 n <= 44, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the nth term of OEIS A106603. */
seq_t a(idx_t n)
{
    /* Base cases for n = 0,1,2. */
    if (n < 3) {
        switch ((idx_t)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)1;
            case 2: return (seq_t)-2;
        }
    }

    /* Static storage for memoised values. */
    static seq_t *dp   = NULL;
    static idx_t  cap  = 0;   /* allocated capacity of dp */
    static idx_t  computed = 0; /* number of values already computed in dp */

    /* Initialise if first use. */
    if (dp == NULL) {
        cap = 3;
        dp = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!dp) return 0;                 /* out of memory – unlikely in tests */
        dp[0] = (seq_t)1;
        dp[1] = (seq_t)1;
        dp[2] = (seq_t)-2;
        computed = 3;                      /* 3 values computed (indices 0..2)  */
    }

    /* Grow the array if necessary. */
    if (n + 1 > cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;                 /* out of memory – unlikely in tests */
        dp = tmp;
        cap = new_cap;
    }

    /* Compute any missing values up to n. */
    if (n + 1 > computed) {
        for (idx_t i = computed; i <= n; ++i) {
            /* a(i) = -2*a(i-1) - 8*a(i-3)   */
            dp[i] = -(seq_t)2 * dp[i-1] - (seq_t)8 * dp[i-3];
        }
        computed = n + 1;
    }

    return dp[n];
}
