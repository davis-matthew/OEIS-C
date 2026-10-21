/*
 * A229842  Number of nX3 0..2 arrays x(i,j) with each element horizontally or
 *          vertically next to at least one element with value 2-x(i,j).
 * https://oeis.org/A229842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays with elements 0..2 such that each
 *            element has a horizontal or vertical neighbour with value
 *            2−x(i,j). Computed by dynamic programming over columns
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Enumerate the 27 possible column patterns. */
    int pat[27][3];
    int id = 0;
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c) {
                pat[id][0] = a;
                pat[id][1] = b;
                pat[id][2] = c;
                ++id;
            }

    /* For each pattern, mask of rows that are already satisfied by vertical
       neighbours in the same column. Bit r set -> row r satisfied. */
    int vertMask[27];
    for (int s = 0; s < 27; ++s) {
        int mask = 0;
        for (int r = 0; r < 3; ++r) {
            int v = pat[s][r];
            int u = 2 - v;          /* complementary value */
            bool sat = false;
            if (r > 0 && pat[s][r - 1] == u) sat = true;
            if (r < 2 && pat[s][r + 1] == u) sat = true;
            if (sat) mask |= 1 << r;
        }
        vertMask[s] = mask;
    }

    /* Initial unsatisfied mask for the first (leftmost) column. */
    int initMask[27];
    for (int s = 0; s < 27; ++s)
        initMask[s] = (~vertMask[s]) & 0x7;    /* keep only 3 bits */

    /* satByCurr[prev][curr] – rows where curr pattern has a horizontal
       neighbour that satisfies the corresponding row of prev. */
    int satByCurr[27][27];
    for (int prev = 0; prev < 27; ++prev)
        for (int curr = 0; curr < 27; ++curr) {
            int mask = 0;
            for (int r = 0; r < 3; ++r)
                if (pat[curr][r] == 2 - pat[prev][r])
                    mask |= 1 << r;
            satByCurr[prev][curr] = mask;
        }

    /* DP arrays: dp[state][mask] – number of ways to reach current column. */
    seq_t dp[27][8] = {0};
    seq_t next[27][8] = {0};

    /* Base: first column. */
    for (int s = 0; s < 27; ++s)
        dp[s][initMask[s]] = (seq_t)1;

    /* Process remaining columns. */
    for (idx_t step = 1; step < n; ++step) {
        /* Reset next */
        for (int s = 0; s < 27; ++s)
            for (int m = 0; m < 8; ++m)
                next[s][m] = (seq_t)0;

        /* Transition */
        for (int prev = 0; prev < 27; ++prev) {
            for (int mPrev = 0; mPrev < 8; ++mPrev) {
                seq_t ways = dp[prev][mPrev];
                if (ways == 0) continue;

                for (int curr = 0; curr < 27; ++curr) {
                    /* All unsatisfied rows of prev must be satisfied by curr. */
                    if ((mPrev & ~satByCurr[prev][curr]) != 0) continue;

                    int sat = vertMask[curr] | satByCurr[prev][curr];
                    int mCurr = (~sat) & 0x7;       /* remaining unsatisfied rows */

                    next[curr][mCurr] += ways;
                }
            }
        }

        /* Copy next to dp for next iteration. */
        for (int s = 0; s < 27; ++s)
            for (int m = 0; m < 8; ++m)
                dp[s][m] = next[s][m];
    }

    /* Sum over states where no rows are unsatisfied. */
    seq_t result = (seq_t)0;
    for (int s = 0; s < 27; ++s)
        result += dp[s][0];

    return result;
}
