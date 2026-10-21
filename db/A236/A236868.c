/*
 * A236868  Number of (n+1)X(1+1) 0..2 arrays with the maximum plus the upper
 *          median minus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A236868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2×(n+1) arrays over {0,1,2} such that for every
 *            2×2 subblock the value max + upper median - lower median is
 *            constant across all subblocks. Computed via dynamic programming
 *            on the 9 possible column states (top,bottom) and the 5 possible
 *            edge weights (0..4)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute adjacency matrix for each possible weight (0..4). */
    static bool adj[5][9][9];
    static bool initialized = false;
    if (!initialized) {
        for (int st1 = 0; st1 < 9; ++st1) {
            int a = st1 / 3;
            int b = st1 % 3;
            for (int st2 = 0; st2 < 9; ++st2) {
                int c = st2 / 3;
                int d = st2 % 3;
                int v[4] = {a, b, c, d};
                /* simple insertion sort of 4 elements */
                for (int i = 0; i < 4; ++i)
                    for (int j = i + 1; j < 4; ++j)
                        if (v[i] > v[j]) {
                            int tmp = v[i];
                            v[i] = v[j];
                            v[j] = tmp;
                        }
                int x4 = v[3];
                int x3 = v[2];
                int x2 = v[1];
                int weight = x4 + x3 - x2; /* 0..4 */
                adj[weight][st1][st2] = true;
            }
        }
        initialized = true;
    }

    /* DP arrays: dp[weight][state], next[weight][state] */
    seq_t dp[5][9] = {0};
    seq_t next[5][9] = {0};

    for (int w = 0; w < 5; ++w)
        for (int s = 0; s < 9; ++s)
            dp[w][s] = 1;   /* sequences of length 1 (no edges) */

    /* iterate n edges */
    for (idx_t step = 0; step < n; ++step) {
        /* reset next */
        for (int w = 0; w < 5; ++w)
            for (int s = 0; s < 9; ++s)
                next[w][s] = 0;

        for (int w = 0; w < 5; ++w) {
            for (int s = 0; s < 9; ++s) {
                seq_t val = dp[w][s];
                if (val == 0) continue;
                for (int ns = 0; ns < 9; ++ns) {
                    if (adj[w][s][ns]) {
                        next[w][ns] += val;
                    }
                }
            }
        }

        /* copy next to dp */
        for (int w = 0; w < 5; ++w)
            for (int s = 0; s < 9; ++s)
                dp[w][s] = next[w][s];
    }

    /* sum over all weights */
    seq_t result = 0;
    for (int w = 0; w < 5; ++w)
        for (int s = 0; s < 9; ++s)
            result += dp[w][s];

    return result;
}
