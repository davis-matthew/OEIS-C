/*
 * A084447  Number of triangular partitions of n of order 5.
 * https://oeis.org/A084447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in
 *            1/((1-x)^5*(1-x^3)^4*(1-x^5)^3*(1-x^7)^2*(1-x^9))
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 62, u64 n <= 401,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of triangular partitions of n of order 5 */
/* Each part size has a fixed number of distinguishable colours:
   1×5, 3×4, 5×3, 7×2, 9×1.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Weights of the 15 distinct part types. */
    static const idx_t weights[] = {
        1,1,1,1,1,           /* 5 types of size 1 */
        3,3,3,3,             /* 4 types of size 3 */
        5,5,5,               /* 3 types of size 5 */
        7,7,                  /* 2 types of size 7 */
        9                      /* 1 type of size 9 */
    };
    const size_t type_count = sizeof(weights)/sizeof(weights[0]);

    /* Allocate a DP array of size n+1. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;   /* Allocation failure – return 0. */

    /* Initialise dp[0] = 1, others 0. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* Unbounded knapsack for each distinct part type. */
    for (size_t t = 0; t < type_count; ++t) {
        idx_t w = weights[t];
        for (idx_t s = w; s <= n; ++s) {
            dp[s] = dp[s] + dp[s - w];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
