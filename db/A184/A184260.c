/*
 * A184260  Number of strings of numbers x(i=1..5) in 0..n with sum i^3*x(i)
 *          equal to 125*n.
 * https://oeis.org/A184260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of 5‑tuples (x1,…,x5) with 0≤xi≤n and x1 + 8x2 + 27x3 +
 *            64x4 + 125x5 = 125 n. Implementation uses bounded‑knapsack DP
 *            with sliding windows
 * Range:     u8 n <= 12, u16 n <= 52, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Special case: for n == 0, equation becomes 0 = 0, only empty tuple */
    if (n == 0) return 0;

    /* Target sum */
    idx_t target = 125 * n;          /* 125 n   */
    idx_t maxsum = target;           /* largest sum achievable with bounds */
    size_t len = (size_t)(maxsum + 1);

    /* Allocate DP arrays (zero‑initialized) */
    seq_t *dp   = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *newdp= (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp || !newdp) {
        /* Allocation failure – return 0 (unlikely with expected limits) */
        if (dp) free(dp);
        if (newdp) free(newdp);
        return 0;
    }

    dp[0] = 1;   /* base case: 0 sum can be achieved in 1 way (all xi=0) */

    /* Process variables with weights 1,8,27,64,125 */
    for (idx_t i = 1; i <= 5; ++i) {
        idx_t w = (idx_t)i * i * i;   /* weight = i³ */

        /* Sliding‑window DP over residues modulo w */
        for (idx_t r = 0; r < w; ++r) {
            seq_t sum = 0;
            idx_t t = 0;                    /* number of steps taken in this residue */

            for (idx_t j = r; j <= maxsum; j += w, ++t) {
                sum += dp[j];
                if (t > n) {
                    sum -= dp[j - w * (n + 1)];
                }
                newdp[j] = sum;
            }
        }

        /* Prepare for next variable */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
        /* Zero the array that will be reused next iteration */
        memset(newdp, 0, len * sizeof(seq_t));
    }

    seq_t result = dp[target];

    free(dp);
    free(newdp);
    return result;
}
