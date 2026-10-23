/*
 * A029409  Expansion of 1/((1-x^5)*(1-x^9)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partition a(n) of n into parts 5, 9, 10, and 11 */
/* The implementation simply performs the classic coin‑change DP
   i.e. a[n] = number of solutions of:
        5*x1 + 9*x2 + 10*x3 + 11*x4 = n
   using 64‑bit unsigned indices and seq_t summation.
   For each call a fresh temporary array of size n+1 is allocated,
   the DP is run, the result extracted, and the array freed. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* allocate space for DP table */
    size_t size = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (dp == NULL)
        return (seq_t)0;         /* allocation failure – unlikely for small n */

    /* initialise table */
    for (size_t i = 0; i < size; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;            /* empty sum */

    /* part sizes */
    const int parts[4] = {5, 9, 10, 11};

    /* run the classic coin‑change DP */
    for (int p = 0; p < 4; ++p) {
        idx_t coin = (idx_t)parts[p];
        for (idx_t sum = coin; sum <= n; ++sum)
            dp[sum] += dp[sum - coin];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
