/*
 * A107841  Series reversion of x*(1-3*x)/(1-x).
 * https://oeis.org/A107841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; for n≥1, a(n) = 3 * Σ_{k=0}^{n-1} a(k) * a(n-1-k) –
 *            a(n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t cap = 0;
    static idx_t sz  = 0;

    /* Ensure capacity for indices up to n (inclusive) */
    if (n >= cap) {
        idx_t newcap = cap ? cap << 1 : 16;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        dp   = tmp;
        cap  = newcap;
    }

    /* Initialise memo table if empty */
    if (sz == 0) {  /* first term */
        dp[0] = (seq_t)1;
        sz = 1;
    }

    /* Compute terms up to n if not already done */
    for (idx_t i = sz; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 0; j <= i-1; ++j) {
            sum += dp[j] * dp[i-1-j];
        }
        dp[i] = sum * (seq_t)3 - dp[i-1];
    }
    sz = n + 1;

    return dp[n];
}
