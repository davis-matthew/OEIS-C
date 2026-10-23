/*
 * A025884  Expansion of 1/((1-x^5)*(1-x^7)*(1-x^10)).
 * https://oeis.org/A025884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of partitions of n into parts 5, 7, and 10 (OEIS A025884)
 * Range:     u8 n <= 399, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for calloc and free */

seq_t a(idx_t n)
{
    /* Allocate a dynamic programming table of length n+1.  The
       allocation is done with calloc so the elements are initialized
       to zero.  The pointer is cast explicitly, so the code compiles
       both as C11 and as C++17. */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;       /* memory error fallback */

    dp[0] = (seq_t)1;               /* empty partition */

    const idx_t coins[3] = {5, 7, 10};

    for (int c = 0; c < 3; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
