/*
 * A029435  Expansion of 1/((1-x^7)(1-x^8)(1-x^10)(1-x^11)).
 * https://oeis.org/A029435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n using parts 7, 8, 10, 11
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Calculates a(n) for OEIS sequence A029435. */
seq_t a(idx_t n)
{
    /* Special case for the zero index. */
    if (n == 0ULL)
        return (seq_t)1;

    /* Allocate an array of seq_t to store DP values. */
#ifdef __cplusplus
    /* In C++ use new[] which calls constructors. */
    seq_t *dp = new seq_t[n + 1];
#else
    /* In C use malloc, casting the void * result. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)                 /* out of memory */
        return (seq_t)0;
#endif

    /* Initialise the DP array. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Parts allowed in the partition. */
    const idx_t parts[4] = {7, 8, 10, 11};

    /* Dynamic programming over the allowed parts. */
    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i)
            dp[i] = dp[i] + dp[i - p];
    }

    seq_t result = dp[n];

    /* Free the allocated memory. */
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}
