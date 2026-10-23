/*
 * A017882  Expansion of 1/(1-x^9-x^10-x^11-x^12-x^13-x^14-x^15).
 * https://oeis.org/A017882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-9)+a(n-10)+a(n-11)+a(n-12)+a(n-13)+a(n-14)+a(n-15)
 * Range:     u8 n <= 47, u16 n <= 81, u32 n <= 147, u64 n <= 280,
 *            u128 n <= 546, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0) = 1; a(1) = a(2) = … = a(8) = 0; a(9)=…=a(15)=1   */
/* Exact computation, memoised per call.  Works for any seq_t type. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static table containing already computed values */
    static seq_t *dp = NULL;
    /* largest index that has been computed */
    static idx_t max_n = 0;

    /* first call – allocate array big enough for the requested n */
    if (dp == NULL) {
        size_t sz = (size_t)(n + 1);
        dp = (seq_t*)calloc(sz, sizeof(seq_t));
        if (dp == NULL) return (seq_t)0;  /* allocation failed */

        /* initialise the first nine values, but only those that exist */
        dp[0] = 1;
        for (idx_t i = 1; i <= n && i < 9; ++i)
            dp[i] = 0;

        max_n = n;
    }
    /* if we need values beyond what we have, extend the table and compute */
    else if (n > max_n) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(dp, new_sz * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failed */
        dp = tmp;

        /* compute indices from max_n+1 up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t sum = 0;
            if (i >= 9)  sum += dp[i - 9];
            if (i >=10)  sum += dp[i -10];
            if (i >=11)  sum += dp[i -11];
            if (i >=12)  sum += dp[i -12];
            if (i >=13)  sum += dp[i -13];
            if (i >=14)  sum += dp[i -14];
            if (i >=15)  sum += dp[i -15];
            dp[i] = sum;
        }
        max_n = n;
    }
    /* n <= max_n: answer already computed */
    return dp[n];
}
