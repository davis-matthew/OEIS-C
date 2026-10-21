/*
 * A237083  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the upper
 *          median plus the lower median minus the minimum of every 2X2
 *          subblock equal.
 * https://oeis.org/A237083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A237083: Number of (n+1)×2 arrays over {0,1,2,3} such that for every
   2×2 subblock the quantity
          max + upper median + lower median – min
   is the same for all subblocks.
   The implementation uses dynamic programming: for each possible
   pair of consecutive rows we precalculate the value of the expression,
   then count, by repeated transitions, all row‑sequences that keep the
   same value for all pairs. */ 

#include <stdint.h>
#include <stddef.h>

static uint8_t SMatrix[16][16];          /* S for each ordered pair of rows */
static int SMatrix_computed = 0;

/* Compute SMatrix[r0][r1] = max + upper + lower - min for rows r0, r1. */
static void compute_SMatrix(void)
{
    if (SMatrix_computed) return;
    for (int r0 = 0; r0 < 16; ++r0) {
        /* decode the two column values of row r0 (base‑4 digits) */
        int a0 = r0 / 4;          /* first column */
        int b0 = r0 % 4;          /* second column */
        for (int r1 = 0; r1 < 16; ++r1) {
            int a1 = r1 / 4;
            int b1 = r1 % 4;
            /* frequency of each value in the 2×2 block */
            uint8_t freq[4] = {0, 0, 0, 0};
            ++freq[a0]; ++freq[b0]; ++freq[a1]; ++freq[b1];
            /* minimum */
            int vmin = 0;
            while (vmin < 4 && freq[vmin] == 0) ++vmin;
            /* maximum */
            int vmax = 3;
            while (vmax >= 0 && freq[vmax] == 0) --vmax;
            /* lower and upper medians */
            int cum = 0, lower = -1, upper = -1;
            for (int v = 0; v < 4; ++v) {
                cum += freq[v];
                if (lower == -1 && cum >= 2) lower = v;
                if (upper == -1 && cum >= 3) upper = v;
            }
            int S = vmax + upper + lower - vmin;    /* 0 ≤ S ≤ 9 */
            SMatrix[r0][r1] = (uint8_t)S;
        }
    }
    SMatrix_computed = 1;
}

seq_t a(idx_t n)
{
    compute_SMatrix();

    /* DP tables: dp_cur[row][S] = number of ways to end at 'row' with
       constant value S after the current number of subblocks. */
    seq_t dp_cur[16][10];
    seq_t dp_next[16][10];

    /* initialise to zero */
    for (int row = 0; row < 16; ++row)
        for (int S = 0; S < 10; ++S)
            dp_cur[row][S] = 0;

    /* first subblock (n = 1) : all ordered pairs of rows */
    for (int r0 = 0; r0 < 16; ++r0) {
        for (int r1 = 0; r1 < 16; ++r1) {
            uint8_t S = SMatrix[r0][r1];
            ++dp_cur[r1][S];      /* each pair contributes 1 */
        }
    }

    if (n == 1) {
        /* total number of 2×2 arrays */
        seq_t total = 0;
        for (int row = 0; row < 16; ++row)
            for (int S = 0; S < 10; ++S)
                total += dp_cur[row][S];
        return total;
    }

    /* subsequent subblocks (n ≥ 2) */
    for (idx_t step = 2; step <= n; ++step) {
        /* zero dp_next */
        for (int row = 0; row < 16; ++row)
            for (int S = 0; S < 10; ++S)
                dp_next[row][S] = 0;

        /* transition: for each current state, add a compatible next row */
        for (int cur_row = 0; cur_row < 16; ++cur_row) {
            for (int S = 0; S < 10; ++S) {
                seq_t cur_val = dp_cur[cur_row][S];
                if (cur_val == 0) continue;
                for (int nxt_row = 0; nxt_row < 16; ++nxt_row) {
                    uint8_t Snew = SMatrix[cur_row][nxt_row];
                    if (Snew == S) {
                        dp_next[nxt_row][S] += cur_val;
                    }
                }
            }
        }

        /* copy dp_next to dp_cur for next iteration */
        for (int row = 0; row < 16; ++row)
            for (int S = 0; S < 10; ++S)
                dp_cur[row][S] = dp_next[row][S];
    }

    /* sum all final states */
    seq_t total = 0;
    for (int row = 0; row < 16; ++row)
        for (int S = 0; S < 10; ++S)
            total += dp_cur[row][S];
    return total;
}
