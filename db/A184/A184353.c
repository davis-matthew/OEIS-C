/*
 * A184353  Number of strings of numbers x(i=1..7) in 0..n with sum i^4*x(i)
 *          equal to 2401*n.
 * https://oeis.org/A184353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions of Σ_{i=1}^7 i^4 · x(i) = 2401·n with
 *            0 ≤ x(i) ≤ n. This is computed by a bounded‑knapsack DP over the
 *            first six variables; for each possible value of x(7) we add the
 *            corresponding number of solutions for the remaining six
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 146, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;          /* offset is 1, but stay safe      */

    /* 2401 = 7^4, maximum possible sum for the first six variables */
    const idx_t target = 2401ULL * n;     /* target sum for the first six vars   */
    const size_t tgt_sz = (size_t)target + 1;

    /* Allocate two arrays of 64‑bit integers for the DP.
       The memory consumption is about 2 * target * 8 bytes. */
    uint64_t *dp = (uint64_t *)calloc(tgt_sz, sizeof(uint64_t));
    uint64_t *ndp = (uint64_t *)calloc(tgt_sz, sizeof(uint64_t));
    if (!dp || !ndp) {                    /* allocation failed */
        free(dp);
        free(ndp);
        return 0;
    }

    dp[0] = 1;                            /* one way to reach sum 0         */

    /* Weights of the first six variables: 1^4,...,6^4 */
    static const idx_t weight[6] = {1, 16, 81, 256, 625, 1296};

    for (int wi = 0; wi < 6; ++wi) {
        const idx_t w = weight[wi];
        /* Reset ndp for this weight */
        memset(ndp, 0, tgt_sz * sizeof(uint64_t));

        /* Bounded knapsack with sliding window for each residue class */
        for (idx_t r = 0; r < w; ++r) {
            unsigned __int128 sum = 0;     /* may exceed 64 bits during accumulation */
            /* iterate over sums of this residue class (ascending order) */
            for (idx_t s = r; s <= target; s += w) {
                sum += dp[s];
                /* subtract the term that leaves the window of size (n+1) */
                if (s >= (n + 1) * w)
                    sum -= dp[s - (n + 1) * w];
                ndp[s] = (uint64_t)sum;
            }
        }
        /* Prepare for next weight: swap dp and ndp */
        uint64_t *tmp = dp;
        dp = ndp;
        ndp = tmp;
    }

    /* Count all solutions: for each possible x(7)=i (0..n) the remaining
       sum on the first six variables must be exactly 2401·i. */
    seq_t ans = 0;
    for (idx_t i = 0; i <= n; ++i)
        ans += (seq_t)dp[2401ULL * i];

    free(dp);
    free(ndp);
    return ans;
}
