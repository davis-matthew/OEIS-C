/*
 * A029189  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into the parts 2, 4, 9, and 11.
 *            * Computed using classic coin‑change dynamic programming
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate an array of size n+1 to hold the DP counts. */
    size_t size = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(size, sizeof(seq_t));
    if (!dp)
        return 0;          /* Out of memory; return 0 as a safe fallback. */
    dp[0] = 1;              /* There's one way to make sum 0: use no parts. */

    /* The allowed parts. */
    const idx_t parts[] = { 2, 4, 9, 11 };
    const size_t num_parts = sizeof(parts) / sizeof(parts[0]);

    /* Classic coin‑change DP: for each part, add it to all achievable sums. */
    for (size_t p = 0; p < num_parts; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
