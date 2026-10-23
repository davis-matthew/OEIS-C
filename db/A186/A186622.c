/*
 * A186622  Number of (n+2)X3 0..2 arrays with 3X3 subblock sums nondecreasing
 *          rightwards and downwards.
 * https://oeis.org/A186622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 15, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = ∏_{c=0}^{2} T_c(L_c),
 * where L_c = (n+2 + 2 - c) / 3 (integer division, the number of rows
 * whose index ≡ c (mod 3)), and T_c(m) is the weighted count of
 * nondecreasing sequences of length m over {0,…,6} with weights
 *
 *   w = {1, 3, 6, 7, 6, 3, 1}
 *
 * i.e.  T_c(m) = Σ_{v_1≤…≤v_m} ∏_{j=1}^{m} w[v_j].
 *
 * This follows from the observation that for a (n+2)×3 array over {0,1,2}
 * the 3×3 subblock sums are just sums of three successive rows,
 * and the nondecreasing condition reduces to r_k ≤ r_{k+3} for every k,
 * which implies that each residue class modulo 3 gives an independent
 * nondecreasing subsequence of row-sums.
 *
 * The algorithm implements the above formula using dynamic programming
 * for each class separately.
 */

#include <stdbool.h>
#include <stdint.h>

/* Row-sum weights: number of 3‑tuples of {0,1,2} summing to r (0 ≤ r ≤ 6) */
static const seq_t row_weights[7] = {1, 3, 6, 7, 6, 3, 1};

seq_t a(idx_t n)
{
    /* Total rows */
    idx_t L = n + 2;
    seq_t result = 1;

    for (idx_t c = 0; c < 3; ++c) {
        /* Length of the c‑th residue class (0‑based indexing) */
        idx_t len = (L + 2 - c) / 3;
        if (len == 0) continue;           /* empty class */

        /* DP arrays: dp[v] = weighted count of sequences ending with value v */
        seq_t dp[7];
        for (int v = 0; v < 7; ++v) dp[v] = row_weights[v];

        /* Extend sequence lengths from 2 up to len */
        for (idx_t l = 2; l <= len; ++l) {
            seq_t prefix[7];
            prefix[0] = dp[0];
            for (int v = 1; v < 7; ++v) prefix[v] = prefix[v - 1] + dp[v];

            seq_t next[7];
            for (int v = 0; v < 7; ++v)
                next[v] = row_weights[v] * prefix[v];   /* multiplicative weight */

            for (int v = 0; v < 7; ++v) dp[v] = next[v];
        }

        /* Total for this class */
        seq_t class_total = 0;
        for (int v = 0; v < 7; ++v) class_total += dp[v];

        result *= class_total;
    }

    return result;
}
