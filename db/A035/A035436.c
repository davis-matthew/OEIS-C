/*
 * A035436  Number of partitions of n into parts 7k+3 or 7k+5.
 * https://oeis.org/A035436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to 3 or 5
 *            mod 7
 * Range:     u8 n <= 65, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate dp array of length n+1, initialized to 0 */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (dp == NULL) return (seq_t)0;   /* allocation failure fallback */

    dp[0] = (seq_t)1;

    /* Add parts of the form 7k + 3 */
    for (idx_t p = 3; p <= n; p += 7)
    {
        for (idx_t j = p; j <= n; ++j)
            dp[j] += dp[j - p];
    }

    /* Add parts of the form 7k + 5 */
    for (idx_t p = 5; p <= n; p += 7)
    {
        for (idx_t j = p; j <= n; ++j)
            dp[j] += dp[j - p];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
