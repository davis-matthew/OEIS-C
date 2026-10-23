/*
 * A029211  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into the parts {2,5,10,11}
 *            computed by dynamic programming. a(0)=1
 * Range:     u8 n <= 105, u16 n <= 742, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Quick return for the base case */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Allocate DP table of size n+1 */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (dp == NULL) {
        return (seq_t)0;  /* allocation failure */
    }

    dp[0] = (seq_t)1;        /* empty partition */

    /* The allowed part sizes */
    static const idx_t parts[4] = { (idx_t)2, (idx_t)5, (idx_t)10, (idx_t)11 };

    /* Fill the DP table */
    for (unsigned p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
