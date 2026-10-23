/*
 * A183956  Number of strings of numbers x(i=1..5) in 0..n with sum i^2*x(i)
 *          equal to n*25.
 * https://oeis.org/A183956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count solutions to 1*x1 + 4*x2 + 9*x3 + 16*x4 + 25*x5 = 25*n,
 *            with 0 ≤ xi ≤ n
 * Range:     u8 n <= 7, u16 n <= 35, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm implements a bounded coin‑change DP.  It works for all
   seq_t/idx_t types supported by the build and is O(5·25·n) in time
   and O(25·n) in space. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* maximum sum that can appear is 25*n */
    idx_t maxSum = 25 * n;
    size_t sz = (size_t)maxSum + 1;

    /* allocate two arrays to alternately hold DP states */
    seq_t *dp   = (seq_t*)calloc(sz, sizeof(seq_t));   /* dp[0] = 1 */
    seq_t *ndp  = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!dp || !ndp) {
        free(dp);
        free(ndp);
        return (seq_t)0;
    }
    dp[0] = (seq_t)1;  /* base case: empty tuple gives sum 0 */

    /* weights of the variables */
    const idx_t weights[5] = {1, 4, 9, 16, 25};

    for (int wi = 0; wi < 5; ++wi) {
        idx_t w = weights[wi];
        /* clear ndp */
        memset(ndp, 0, sz * sizeof(seq_t));

        /* sliding window DP for a bounded number (0..n) of weight w */
        for (idx_t r = 0; r < w; ++r) {
            seq_t window = 0;
            idx_t k = 0;
            for (idx_t s = r; s <= maxSum; s += w, ++k) {
                window += dp[s];
                if (k > n) {
                    /* subtract the element falling out of the window */
                    window -= dp[s - w * (n + 1)];
                }
                ndp[s] = window;
            }
        }

        /* swap dp and ndp for next iteration */
        seq_t *tmp = dp;
        dp = ndp;
        ndp = tmp;
    }

    /* result: coefficient for sum 25*n */
    seq_t ans = dp[maxSum];

    /* free temporary arrays */
    free(dp);
    free(ndp);
    return ans;
}
