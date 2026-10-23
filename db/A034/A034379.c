/*
 * A034379  Expansion of 1/(1-x)^2/(1-x^2)/(1-x^3)/(1-x^5)/(1-x^8).
 * https://oeis.org/A034379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of the expansion of
 *            1/((1-x)^2*(1-x^2)*(1-x^3)*(1-x^5)*(1-x^8))
 * Range:     u8 n <= 14, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A034379 as described above.  Uses a simple dynamic program
   that iterates over the part sizes 1,1,2,3,5,8, adding the number of
   ways to reach each value.  No memoization is required; the cost is
   O(n).  Works for all builds support of seq_t, including arbitrary‑
   precision types. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate an array that holds all intermediate counts up to n. */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(len, sizeof(seq_t));
    if (dp == NULL)
        return 0;  /* allocation failure – return 0 as a fallback */

    /* Base case: one way to form the empty sum. */
    dp[0] = 1;

    /* The part sizes, with weight 1 appearing twice to represent the
       squared generating function for 1/(1-x)^2. */
    const int weights[6] = {1, 1, 2, 3, 5, 8};

    for (int w_idx = 0; w_idx < 6; ++w_idx) {
        idx_t w = (idx_t)weights[w_idx];
        for (idx_t i = w; i <= n; ++i) {
            dp[i] += dp[i - w];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
