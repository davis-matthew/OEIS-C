/*
 * A029316  Expansion of 1/((1-x^3)*(1-x^8)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A029316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-8) + a(n-9) + a(n-10) with a(0) = 1 and a(k)
 *            = 0 for k < 0 (number of partitions of n into parts 3, 8, 9, 10)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* calloc, free */

seq_t a(idx_t n)
{
    /* Allocate an array of seq_t of length n+1 and zero‑initialize it. */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp) return 0;           /* allocation failure – unlikely for small n */

    dp[0] = 1;   /* one way to make 0: choose nothing */

    /* Process each part type in the outer loop. This guarantees that the
       order of parts is irrelevant, counting combinations only. */
    const idx_t parts[4] = {3, 8, 9, 10};
    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t res = dp[n];
    free(dp);
    return res;
}
