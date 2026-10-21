/*
 * A226254  Number of ways of writing n as the sum of 10 triangular numbers
 *          from A000217.
 * https://oeis.org/A226254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as the sum of 10 triangular
 *            numbers (triangular numbers are 0,1,3,6,10,…)
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 320, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external integer types provided by the build */
seq_t a(idx_t n)
{
    /* generate list of triangular numbers ≤ n */
    idx_t max_m = 0;
    while (((seq_t)max_m * (max_m + 1)) / 2 <= n)
        ++max_m;                     /* now 0 ≤ m < max_m are admissible indices */

    idx_t *tri = (idx_t *)malloc((size_t)max_m * sizeof(idx_t));
    if (!tri) return 0;               /* out‑of‑memory fallback */

    for (idx_t m = 0; m < max_m; ++m)
        tri[m] = (idx_t)(((seq_t)m * (m + 1)) / 2);

    /* dynamic programming arrays: dp[s] = ways to reach sum s with k terms */
    seq_t *dp   = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *newdp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp || !newdp) {             /* out‑of‑memory fallback */
        free(tri); free(dp); free(newdp);
        return 0;
    }

    dp[0] = 1;                        /* base case: empty sum */

    for (unsigned int step = 0; step < 10; ++step) {
        for (idx_t s = 0; s <= n; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            for (idx_t k = 0; k < max_m; ++k) {
                idx_t t = tri[k];
                idx_t idx = s + t;
                if (idx > n) break;
                newdp[idx] += val;
            }
        }
        /* prepare for next iteration */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
        for (idx_t i = 0; i <= n; ++i) newdp[i] = 0;
    }

    seq_t result = dp[n];

    free(dp);
    free(newdp);
    free(tri);
    return result;
}
