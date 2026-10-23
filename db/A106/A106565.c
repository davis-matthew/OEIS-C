/*
 * A106565  a(n) = 5*a(n-1) + 5*a(n-2) with a(0) = 0, a(1) = 5.
 * https://oeis.org/A106565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 5*a(n-2), a(0) = 0, a(1) = 5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t cap = 0;
    static idx_t sz  = 0;

    /* Initialise storage for the first two terms. */
    if (dp == NULL) {
        /* Allocate room for at least two elements (0 and 1). */
        cap = 2;
        dp = (seq_t *)malloc(cap * sizeof(seq_t));
        if (dp == NULL) abort();
        dp[0] = (seq_t)0;
        dp[1] = (seq_t)5;
        sz = 2;
    }

    if (n < sz)
        return dp[n];

    /* Grow the array until it can hold the requested index. */
    while (n >= cap) {
        idx_t newcap = cap * 2;
        if (newcap <= cap) newcap = cap + 2; /* defensive: avoid overflow */
        seq_t *tmp = (seq_t *)realloc(dp, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();
        dp = tmp;
        cap = newcap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = sz; i <= n; ++i) {
        dp[i] = (seq_t)5 * dp[i - 1] + (seq_t)5 * dp[i - 2];
    }
    sz = n + 1;

    return dp[n];
}
