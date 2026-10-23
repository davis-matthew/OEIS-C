/*
 * A029269  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 789, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of the number of partitions of n into parts 3, 4, 10, and 11
   using the standard coin‑change dynamic‑programming algorithm.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* dp[i] will hold the number of partitions of i using the allowed parts. */
    seq_t *dp = (seq_t *)malloc((n + 1u) * sizeof(seq_t));
    if (dp == NULL)
        return (seq_t)0;          /* allocation failed */

    /* initialise */
    dp[0] = (seq_t)1;
    for (idx_t i = 1u; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* parts that may be used; process each part fully (outer loop) */
    const idx_t parts[] = {3u, 4u, 10u, 11u};
    for (int p = 0; p < 4; ++p)
    {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i)
            dp[i] = dp[i] + dp[i - part];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
