/*
 * A183367  Number of nX5 binary arrays with no element equal to the sum mod 3
 *          of its horizontal and vertical neighbors.
 * https://oeis.org/A183367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary matrices in which no element equals
 *            the sum (mod 3) of its horizontal and vertical neighbors
 *            (non‑existent neighbors are treated as 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a two‑row transfer matrix (states are pairs of
   consecutive rows) and precomputes all valid transitions.  The DP runs
   in O(n·32³) ≈ 3·10⁸ simple operations for n=10 000, which is well
   inside the time limit. */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* skip n < 1 – behaviour not specified for offset (index 1) */
    if (n == 0) return (seq_t)0;

    /* Precompute all valid transitions */
    static bool init_done = false;
    static bool valid[32][32][32];
    if (!init_done) {
        for (int pp = 0; pp < 32; ++pp) {
            for (int p = 0; p < 32; ++p) {
                for (int c = 0; c < 32; ++c) {
                    bool ok = true;
                    for (int j = 0; j < 5 && ok; ++j) {
                        int up = (pp >> j) & 1;
                        int cur = (p >> j) & 1;
                        int left = (j > 0) ? ((p >> (j-1)) & 1) : 0;
                        int right = (j < 4) ? ((p >> (j+1)) & 1) : 0;
                        int down = (c >> j) & 1;
                        int sum = up + left + right + down;
                        int mod = sum % 3;
                        if (mod <= 1 && cur == mod) ok = false;
                    }
                    valid[pp][p][c] = ok;
                }
            }
        }
        init_done = true;
    }

    /* DP arrays: dp[prev_prev][prev] */
    seq_t dp[32][32];
    seq_t ndp[32][32];
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* first row: prev_prev = 0 sentinel */
    for (int row0 = 0; row0 < 32; ++row0)
        dp[0][row0] = (seq_t)1;

    /* Process remaining rows */
    for (idx_t r = 1; r < n; ++r) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                ndp[i][j] = 0;

        for (int pp = 0; pp < 32; ++pp) {
            for (int p = 0; p < 32; ++p) {
                seq_t ways = dp[pp][p];
                if (ways == 0) continue;
                for (int c = 0; c < 32; ++c) {
                    if (!valid[pp][p][c]) continue;
                    ndp[p][c] += ways;
                }
            }
        }

        /* copy ndp back to dp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Final transition to sentinel zeros (down neighbor = 0) */
    seq_t total = 0;
    for (int pp = 0; pp < 32; ++pp) {
        for (int p = 0; p < 32; ++p) {
            seq_t ways = dp[pp][p];
            if (ways == 0) continue;
            if (valid[pp][p][0]) /* curr = 0 sentinel */
                total += ways;
        }
    }
    return total;
}
