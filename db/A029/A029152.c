/*
 * A029152  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^6)*(1-x^9)).
 * https://oeis.org/A029152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 3, 6, and 9. This
 *            implements the standard unbounded‑coin DP
 * Range:     u8 n <= 68, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The allowed part sizes. */
    static const idx_t parts[4] = { 2, 3, 6, 9 };

#ifdef __cplusplus
    /* C++ build: use new/delete */
    seq_t *dp = new seq_t[(size_t)n + 1];
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    for (int p = 0; p < 4; ++p) {
        idx_t v = parts[p];
        for (idx_t i = v; i <= n; ++i)
            dp[i] = dp[i] + dp[i - v];
    }

    seq_t result = dp[n];
    delete [] dp;
    return result;
#else
    /* C build: use malloc/free */
    seq_t *dp = malloc(((size_t)n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;   /* allocation failed */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    for (int p = 0; p < 4; ++p) {
        idx_t v = parts[p];
        for (idx_t i = v; i <= n; ++i)
            dp[i] = dp[i] + dp[i - v];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
#endif
}
