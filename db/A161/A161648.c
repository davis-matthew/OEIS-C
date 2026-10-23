/*
 * A161648  Number of reduced words of length n in the Weyl group A_35.
 * https://oeis.org/A161648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of the polynomial Prod_{k=1..35} (1 + x + … +
 *            x^k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by a standard bounded knapsack DP. */

#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate DP array of size n+1. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) /* allocation failed */
        return 0;

    dp[0] = (seq_t)1;   /* empty sum */

    for (idx_t k = 1; k <= 35; ++k)
    {
        seq_t *next = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!next) {
            free(dp);
            return 0;
        }

        for (idx_t j = 0; j <= n; ++j) {
            idx_t limit = (j < k) ? j : k;   /* max i allowed */
            seq_t sum = 0;
            for (idx_t i = 0; i <= limit; ++i)
                sum += dp[j - i];
            next[j] = sum;
        }

        free(dp);
        dp = next;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
