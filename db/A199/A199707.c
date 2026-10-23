/*
 * A199707  Number of -n..n arrays x(0..4) of 5 elements with zero sum and no
 *          two neighbors equal.
 * https://oeis.org/A199707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 145, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   block_weights
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: counts all 5‑tuples x0..x4 with xi∈[−n,n], sum xi = 0 and no adjacent equalities.
   Inclusion–exclusion over the 4 adjacency edges is used. For each subset of edges we
   form blocks of equal elements, reduce the problem to counting solutions of
   ∑ w_j·u_j = 5n, u_j∈[0,2n] for block weights w_j.  This is solved by a
   sliding‑window convolution DP in O(5n) time per subset.  The final answer is
   ∑_{S⊆{0,1,2,3}} (−1)^{|S|} count_S.  */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/block_weights.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset is 1, but guard anyway */

    idx_t target = 5 * n;          /* maximum weighted sum (5n) */
    /* allocate DP arrays */
    seq_t *dp = (seq_t *)calloc((size_t)(target + 1), sizeof(seq_t));
    seq_t *ndp = (seq_t *)calloc((size_t)(target + 1), sizeof(seq_t));
    if (!dp || !ndp) {
        /* allocation failed – fall back to zero (shouldn't happen in tests) */
        free(dp); free(ndp);
        return (seq_t)0;
    }

    seq_t pos = 0, neg = 0;          /* sums for even/odd masks */

    /* iterate over all 4-bit masks (0..15) */
    for (int mask = 0; mask < 16; ++mask) {
        /* reset dp to initial state: only sum 0 reachable with first variable */
        dp[0] = (seq_t)1;
        if (target > 0) memset(dp + 1, 0, (size_t)target * sizeof(seq_t));

        int weights[5], wlen;
        block_weights(mask, weights, &wlen);

        /* weighted convolution DP */
        idx_t tmax1 = 2 * n + 1; /* number of u values per block (0..2n inclusive) */

        /* for each block (weight w) perform convolution */
        for (int bi = 0; bi < wlen; ++bi) {
            int w = weights[bi];
            /* zero ndp */
            memset(ndp, 0, (size_t)(target + 1) * sizeof(seq_t));

            /* sliding window over each remainder class modulo w */
            for (int r = 0; r < w; ++r) {
                seq_t window = 0;
                idx_t k = 0;
                for (idx_t s = r; s <= target; s += w, ++k) {
                    window += dp[(size_t)s];
                    if (k > n * 2) { /* k > tmax */
                        idx_t sub = s - tmax1 * (idx_t)w;
                        window -= dp[(size_t)sub];
                    }
                    ndp[(size_t)s] = window;
                }
            }
            /* swap dp and ndp for next block */
            seq_t *tmp = dp;
            dp = ndp;
            ndp = tmp;
        }

        /* after processing all blocks, dp contains counts for this mask */
        seq_t cnt = dp[(size_t)target];

        /* inclusion–exclusion accumulation */
        if (__builtin_popcount((unsigned)mask) & 1) {
            neg += cnt;
        } else {
            pos += cnt;
        }
    }

    free(dp);
    free(ndp);

    /* final result: pos - neg (guaranteed non‑negative) */
    return pos - neg;
}
