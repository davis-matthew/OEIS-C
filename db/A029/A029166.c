/*
 * A029166  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 603, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 2,3,9,11 (DP O(n)) */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Allocate and zero the DP table. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return (seq_t)0;            /* unlikely out‑of‑memory */

    dp[0] = (seq_t)1;                    /* one way to make sum 0 */

    const unsigned parts[4] = {2, 3, 9, 11};
    for (unsigned pi = 0; pi < 4; ++pi) {
        idx_t part = (idx_t)parts[pi];
        for (idx_t k = part; k <= n; ++k) {
            dp[k] += dp[k - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
