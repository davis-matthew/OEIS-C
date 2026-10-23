/*
 * A109214  Product of a(n-1) and digit reversal of a(n-2).
 * https://oeis.org/A109214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) * rev(a(n-2)), a(1) = 1, a(2) = 2
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_digits.h"

/* Compute the nth term of OEIS sequence A109214. */
seq_t a(idx_t n)
{
    if (n < 1U) /* undefined for indices below 1 */
        return (seq_t)0;

    /* Memoisation storage: dp[1] … dp[sz] hold already computed terms. */
    static seq_t *dp   = NULL;
    static idx_t  sz   = 0;   /* highest index currently populated */
    static idx_t  cap  = 0;   /* capacity of dp (always >= sz+1) */

    /* Grow the array if necessary. */
    if (n >= cap)
    {
        idx_t new_cap = (cap == 0) ? 4 : cap * 2;
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(dp, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        dp   = tmp;
        cap  = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = sz + 1; i <= n; ++i)
    {
        if (i == 1U)
            dp[1] = (seq_t)1;
        else if (i == 2U)
            dp[2] = (seq_t)2;
        else
            dp[i] = dp[i - 1] * reverse_digits(dp[i - 2]);
    }
    sz = n;   /* record that all terms up to n are now available */

    return dp[n];
}
