/*
 * A238049  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the
 *          minimum minus the upper median minus the lower median of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A238049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by dynamic programming over rows.
   Counts all (n+1)×2 arrays with entries 0..3 such that
   for every 2×2 subblock the expression "max + min − upper_median − lower_median"
   differs by exactly one from its vertical neighbor subblock.
   (There is no horizontal neighbor subblock because the array has two columns.)
   The algorithm uses O(n) time and O(1) additional memory. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑compute the block value for every pair of consecutive rows. */
    static int expr[16][16];
    static int init_done = 0;
    if (!init_done) {
        for (idx_t r0 = 0; r0 < 16; ++r0) {
            int a0 = r0 / 4;
            int b0 = r0 % 4;
            for (idx_t r1 = 0; r1 < 16; ++r1) {
                int a1 = r1 / 4;
                int b1 = r1 % 4;
                int mn = a0; if (b0 < mn) mn = b0; if (a1 < mn) mn = a1; if (b1 < mn) mn = b1;
                int mx = a0; if (b0 > mx) mx = b0; if (a1 > mx) mx = a1; if (b1 > mx) mx = b1;
                int sum = a0 + b0 + a1 + b1;
                expr[r0][r1] = 2 * (mx + mn) - sum;   /* value in [-3,3] */
            }
        }
        init_done = 1;
    }

    /* DP arrays: 16 possible rows, 7 possible expression values (-3..3). */
    seq_t dp_cur[16][7];
    seq_t dp_next[16][7];
    for (idx_t r = 0; r < 16; ++r)
        for (int e = 0; e < 7; ++e) {
            dp_cur[r][e] = 0;
            dp_next[r][e] = 0;
        }

    /* Base: the first block uses rows 0 and 1. */
    for (idx_t r0 = 0; r0 < 16; ++r0)
        for (idx_t r1 = 0; r1 < 16; ++r1) {
            int e = expr[r0][r1];
            dp_cur[r1][e + 3] += 1;
        }

    /* Process remaining blocks (total blocks = n). */
    for (idx_t step = 1; step < n; ++step) {
        /* Zero dp_next for next iteration. */
        for (idx_t r = 0; r < 16; ++r)
            for (int e = 0; e < 7; ++e)
                dp_next[r][e] = 0;

        for (idx_t rprev = 0; rprev < 16; ++rprev)
            for (int eidx = 0; eidx < 7; ++eidx) {
                seq_t ways = dp_cur[rprev][eidx];
                if (ways == 0) continue;
                int eprev = eidx - 3;
                for (idx_t rnext = 0; rnext < 16; ++rnext) {
                    int ecurr = expr[rprev][rnext];
                    int diff = ecurr - eprev;
                    if (diff < 0) diff = -diff;
                    if (diff == 1)
                        dp_next[rnext][ecurr + 3] += ways;
                }
            }

        /* Swap dp_cur and dp_next. */
        for (idx_t r = 0; r < 16; ++r)
            for (int e = 0; e < 7; ++e)
                dp_cur[r][e] = dp_next[r][e];
    }

    /* Sum all valid completions. */
    seq_t result = 0;
    for (idx_t r = 0; r < 16; ++r)
        for (int e = 0; e < 7; ++e)
            result += dp_cur[r][e];

    return result;
}
