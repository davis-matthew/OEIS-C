/*
 * A206102  Number of (n+1) X 2 0..3 arrays with the number of clockwise edge
 *          increases in 2 X 2 subblocks nondecreasing, and counterclockwise
 *          edge increases nonincreasing, rightwards and downwards.
 * https://oeis.org/A206102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of (n+1) × 2 arrays with entries 0–3 such that,
   for each consecutive 2×2 subblock:
     * the clockwise edge‑increase count is nondecreasing,
     * the counter‑clockwise edge‑increase count is nonincreasing.
   Computed by dynamic programming over the rows.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static uint8_t incCW[16][16];
static uint8_t incCC[16][16];
static bool inc_init = false;

static void init_inc(void)
{
    if (inc_init) return;
    for (int r0 = 0; r0 < 16; ++r0) {
        int a = (r0 >> 2) & 3;
        int b = r0 & 3;
        for (int r1 = 0; r1 < 16; ++r1) {
            int c = (r1 >> 2) & 3;
            int d = r1 & 3;
            incCW[r0][r1] = (uint8_t)((b > a) + (d > b) + (c > d) + (a > c));
            incCC[r0][r1] = (uint8_t)((c > a) + (d > c) + (b > d) + (a > b));
        }
    }
    inc_init = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset starts at 1 */

    init_inc();

    seq_t dp[16][5][5];
    seq_t ndp[16][5][5];

    /* zero the dp array */
    for (int r = 0; r < 16; ++r)
        for (int cw = 0; cw < 5; ++cw)
            for (int cc = 0; cc < 5; ++cc)
                dp[r][cw][cc] = (seq_t)0;

    /* first subblock (rows r0, r1) */
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1) {
            uint8_t cw = incCW[r0][r1];
            uint8_t cc = incCC[r0][r1];
            dp[r1][cw][cc] += (seq_t)1;
        }

    /* process remaining subblocks */
    for (idx_t step = 2; step <= n; ++step) {
        /* zero ndp */
        for (int r = 0; r < 16; ++r)
            for (int cw = 0; cw < 5; ++cw)
                for (int cc = 0; cc < 5; ++cc)
                    ndp[r][cw][cc] = (seq_t)0;

        for (int rprev = 0; rprev < 16; ++rprev)
            for (int cw = 0; cw < 5; ++cw)
                for (int cc = 0; cc < 5; ++cc) {
                    seq_t val = dp[rprev][cw][cc];
                    if (val == (seq_t)0) continue;
                    for (int rnext = 0; rnext < 16; ++rnext) {
                        uint8_t ncw = incCW[rprev][rnext];
                        uint8_t ncc = incCC[rprev][rnext];
                        if (ncw >= cw && ncc <= cc)
                            ndp[rnext][ncw][ncc] += val;
                    }
                }

        /* copy ndp -> dp */
        for (int r = 0; r < 16; ++r)
            for (int cw = 0; cw < 5; ++cw)
                for (int cc = 0; cc < 5; ++cc)
                    dp[r][cw][cc] = ndp[r][cw][cc];
    }

    /* sum all valid final states */
    seq_t result = (seq_t)0;
    for (int r = 0; r < 16; ++r)
        for (int cw = 0; cw < 5; ++cw)
            for (int cc = 0; cc < 5; ++cc)
                result += dp[r][cw][cc];

    return result;
}
