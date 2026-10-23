/*
 * A025451  Number of partitions of n into 7 nonnegative cubes.
 * https://oeis.org/A025451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into exactly 7 nonnegative
 *            cubes
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The build supplies seq_t and idx_t. */
seq_t a(idx_t n)
{
    /* Allocate a 2‑D DP array of size (8) × (n+1).  */
    size_t sz = (size_t)n + 1;
    size_t total = 8 * sz;
    seq_t *dp = (seq_t *)malloc(sizeof(seq_t) * total);
    if (!dp) return (seq_t)0;                 /* allocation failure */

    /* initialise the table to 0 */
    for (size_t i = 0; i < total; ++i) dp[i] = (seq_t)0;

    /* base case: 0 parts summing to 0 is 1 way */
    dp[0] = (seq_t)1;                         /* dp[0][0] == 1 */

    /* largest cube not exceeding n */
    idx_t cmax = 0;
    for (idx_t i = 1;; ++i) {
        idx_t cube = i * i * i;
        if (cube > n) break;
        ++cmax;
    }

    /* Dynamic programming over cube values (1^3 … cmax^3). */
    for (idx_t i = 1; i <= cmax; ++i) {
        idx_t cube = i * i * i;               /* cube value */
        size_t cube_idx = (size_t)cube;
        for (idx_t s = cube; s <= n; ++s) {
            size_t s_idx = (size_t)s;
            for (idx_t p = 1; p <= 7; ++p) {
                size_t row = (size_t)p * sz + s_idx;
                size_t prev_row = (size_t)(p - 1) * sz + (s_idx - cube_idx);
                dp[row] += dp[prev_row];
            }
        }
    }

    /* Sum partitions using 0 … 7 positive cubes
       (zero cubes are implicit zeros that pad to 7 parts). */
    seq_t result = (seq_t)0;
    for (idx_t p = 0; p <= 7; ++p) {
        result += dp[(size_t)p * sz + (size_t)n];
    }

    free(dp);
    return result;
}
