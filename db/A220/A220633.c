/*
 * A220633  Number of ways to reciprocally link elements of an 3 X n array
 *          either to themselves or to exactly two horizontal or antidiagonal
 *          neighbors.
 * https://oeis.org/A220633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 2*a(n-3) for n>5 Initial values:
 *            a(1)=1, a(2)=1, a(3)=3, a(4)=12, a(5)=44
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: index 1..len contains computed terms */
    static seq_t *dp = NULL;
    static size_t len = 0;   /* highest index computed */
    static size_t cap = 0;   /* allocated capacity (>= len+1) */

    if (n == 0)
        return (seq_t)0;   /* undefined for 0, but harmless */

    /* initialize cache if not yet done */
    if (dp == NULL) {
        cap = 6;            /* store indices 1..5 + unused 0 */
        dp = (seq_t*)realloc(dp, cap * sizeof(seq_t));
        dp[0] = (seq_t)0;   /* unused */
        dp[1] = (seq_t)1;
        dp[2] = (seq_t)1;
        dp[3] = (seq_t)3;
        dp[4] = (seq_t)12;
        dp[5] = (seq_t)44;
        len = 5;
    }

    if ((idx_t)n <= (idx_t)len)
        return dp[n];     /* already computed */

    /* ensure capacity */
    if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        dp = (seq_t*)realloc(dp, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        dp[i] = (seq_t)(4 * dp[i-1]) - dp[i-2] - (seq_t)(2 * dp[i-3]);
    }
    len = (size_t)n;
    return dp[n];
}
