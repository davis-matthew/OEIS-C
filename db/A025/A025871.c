/*
 * A025871  Expansion of 1/((1-x^4)*(1-x^7)*(1-x^12)).
 * https://oeis.org/A025871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4)+a(n-7)-a(n-11)+a(n-12)-a(n-16)-a(n-19)+a(n-23),
 *            with a(0)=1 and a(k)=0 for k<0. This recurrence counts
 *            partitions of n into parts 4, 7 and 12. The function uses
 *            dynamic programming with memoised table
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *dp   = NULL;
    static idx_t  last = 0;      /* highest index already computed */
    static size_t cap  = 0;      /* allocated capacity */

    /* initialise table on first call */
    if (!dp) {
        cap = 64;
        dp  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!dp) return 0;        /* unlikely: out of memory */
        memset(dp, 0, cap * sizeof(seq_t));
        dp[0] = 1;                /* base case */
    }

    /* grow array if needed */
    if (n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;                /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* out of memory */
        memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        dp = tmp;
        cap = new_cap;
    }

    /* fill table up to n if not already done */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t sum_pos = 0, sum_neg = 0;
            /* positive contributions */
            if (i >= 4)  sum_pos += dp[i - 4];
            if (i >= 7)  sum_pos += dp[i - 7];
            if (i >= 12) sum_pos += dp[i - 12];
            if (i >= 23) sum_pos += dp[i - 23];
            /* negative contributions */
            if (i >= 11) sum_neg += dp[i - 11];
            if (i >= 16) sum_neg += dp[i - 16];
            if (i >= 19) sum_neg += dp[i - 19];
            dp[i] = sum_pos - sum_neg;  /* safe: sum_pos >= sum_neg */
        }
        last = n;
    }

    return dp[n];
}
