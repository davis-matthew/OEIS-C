/*
 * A029403  Expansion of 1/((1-x^5)*(1-x^8)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A029403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-5) + a(n-8) + a(n-9) + a(n-10)
 *            - a(n-13) - a(n-14) - a(n-15) - a(n-17) - a(n-18) - a(n-19)
 *            + a(n-22) + a(n-23) + a(n-24) + a(n-27) - a(n-32) ,
 *            with a(0)=1 and a(k)=0 for 1≤k<5.  This is the coefficient
 *            of xⁿ in 1/((1-x⁵)(1-x⁸)(1-x⁹)(1-x¹⁰)), i.e. the number of
 *            partitions of n into parts 5, 8, 9 and 10.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static table of computed values */
    static seq_t *dp     = NULL; /* array of coefficients    */
    static idx_t cap     = 0;    /* current allocated size   */
    static idx_t last    = 0;    /* last index that has been computed */

    /* grow table if necessary */
    if (n + 1 > cap) {
        seq_t *tmp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        dp  = tmp;
        cap = n + 1;
    }

    /* initialise the very first entry (and the first few zeros) */
    if (last == 0 && cap > 0) {
        dp[0] = 1;
        if (cap > 1) dp[1] = 0;
        if (cap > 2) dp[2] = 0;
        if (cap > 3) dp[3] = 0;
        if (cap > 4) dp[4] = 0;
    }

    /* fill missing entries up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = 0;

        if (i >= 5)  val += dp[i - 5];
        if (i >= 8)  val += dp[i - 8];
        if (i >= 9)  val += dp[i - 9];
        if (i >= 10) val += dp[i -10];

        if (i >= 13) val -= dp[i -13];
        if (i >= 14) val -= dp[i -14];
        if (i >= 15) val -= dp[i -15];
        if (i >= 17) val -= dp[i -17];
        if (i >= 18) val -= dp[i -18];
        if (i >= 19) val -= dp[i -19];

        if (i >= 22) val += dp[i -22];
        if (i >= 23) val += dp[i -23];
        if (i >= 24) val += dp[i -24];
        if (i >= 27) val += dp[i -27];

        if (i >= 32) val -= dp[i -32];

        dp[i] = val;
    }
    last = n;
    return dp[n];
}
