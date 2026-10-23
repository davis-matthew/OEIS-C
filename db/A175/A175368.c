/*
 * A175368  Number of integer 4-tuples (x,y,z,u) satisfying |x|^3 + |y|^3 +
 *          |z|^3 + |u|^3 = n, -n <= x,y,z,u <= n.
 * https://oeis.org/A175368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer 4‑tuples (x,y,z,u) satisfying *
 *            |x|^3+|y|^3+|z|^3+|u|^3 = n, with –n ≤ x,y,z,u ≤ n. * (the bound
 *            is redundant because |x|^3 ≤ n ⇒ |x| ≤ n). * The algorithm
 *            iteratively builds the weighted count of * non‑negative tuples
 *            (|x|,|y|,|z|,|u|) where each weight is * 1 for a zero...
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cube_root_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, calloc, free */
#include <stdio.h>    /* for debugging (optional) */
#include "../utils/cube_root_uint64.h"

seq_t a(idx_t n) {
    /* Special case: n == 0 */
    if (n == 0) return (seq_t)1;

    idx_t k = cube_root_uint64(n);          /* maximum |value| */
    size_t sz = (size_t)(n + 1);            /* array size */

    /* Allocate DP arrays */
#ifdef __cplusplus
    seq_t *dp  = new seq_t[sz];
    seq_t *dp2 = new seq_t[sz];
    std::fill(dp,  dp + sz,  seq_t(0));
    std::fill(dp2, dp2 + sz, seq_t(0));
#else
    seq_t *dp  = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *dp2 = (seq_t*)calloc(sz, sizeof(seq_t));
#endif

    dp[0] = 1;   /* base case: empty tuple sums to 0 */

    for (int var = 0; var < 4; ++var) {     /* four variables */
        /* reset dp2 */
#ifdef __cplusplus
        std::fill(dp2, dp2 + sz, seq_t(0));
#else
        for (size_t i = 0; i < sz; ++i) dp2[i] = 0;
#endif
        for (idx_t s = 0; s <= n; ++s) {
            seq_t cur = dp[s];
            if (cur == 0) continue;
            for (idx_t v = 0; v <= k; ++v) {
                idx_t val = v * v * v;          /* v^3 */
                idx_t ns = s + val;
                if (ns > n) break;
                seq_t weight = (v == 0) ? (seq_t)1 : (seq_t)2;
                dp2[ns] += cur * weight;
            }
        }
        /* swap dp and dp2 for next iteration */
        seq_t *tmp = dp;
        dp = dp2;
        dp2 = tmp;
    }

    seq_t result = dp[n];

    /* Free allocated memory */
#ifdef __cplusplus
    delete[] dp;
    delete[] dp2;
#else
    free(dp);
    free(dp2);
#endif

    return result;
}
