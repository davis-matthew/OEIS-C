/*
 * A116458  Number of partitions of n into parts congruent to 1, 9, or 11 (mod
 *          14).
 * https://oeis.org/A116458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to 1, 9,
 *            or 11 (mod 14)
 * Range:     u8 n <= 70, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for dynamic programming */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        return 0;

    /* Initialize */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Allowed part sizes: 1, 9, 11 modulo 14 */
    for (idx_t part = 1; part <= n; part += 14)
    {
        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
    }
    for (idx_t part = 9; part <= n; part += 14)
    {
        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
    }
    for (idx_t part = 11; part <= n; part += 14)
    {
        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
