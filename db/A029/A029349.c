/*
 * A029349  Expansion of 1/((1-x^4)*(1-x^6)*(1-x^7)*(1-x^10)).
 * https://oeis.org/A029349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029349
   Expansion of 1/((1-x^4)*(1-x^6)*(1-x^7)*(1-x^10)).
   Number of partitions of n into parts 4, 6, 7 and 10.
   Recurrence (with base a(0)=1 and a(k)=0 for k<0):
        a(n) = a(n-4)+a(n-6)+a(n-7)-a(n-11)-a(n-13)-a(n-14)-a(n-16)+a(n-20) */
#include <stdlib.h>

static seq_t *dp = NULL;      /* cache for computed values   */
static idx_t  dp_cap = 0;     /* size of the allocated array */

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough for index n   */
    if (dp_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        dp = tmp;
        dp_cap = new_cap;

        /* compute from scratch up to new_cap-1 */
        dp[0] = (seq_t)1;           /* base case */
        for (size_t i = 1; i < new_cap; ++i) dp[i] = 0;

        /* list of parts */
        static const idx_t parts[4] = {4, 6, 7, 10};
        for (int p = 0; p < 4; ++p) {
            idx_t coin = parts[p];
            for (size_t amt = (size_t)coin; amt < new_cap; ++amt) {
                dp[amt] += dp[amt - coin];
            }
        }
    }
    /* safe because dp_cap > n after possible growth   */
    return dp[(size_t)n];
}
