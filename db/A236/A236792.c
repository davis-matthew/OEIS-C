/*
 * A236792  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the upper
 *          median plus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A236792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   smooth_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence (OEIS A236792):
 * a(n) = 15*a(n-1) - 9*a(n-2) - 880*a(n-3) + 3426*a(n-4)
 *      + 18155*a(n-5) - 121505*a(n-6) - 115924*a(n-7)
 *      + 2003052*a(n-8) - 1313613*a(n-9) - 18570720*a(n-10)
 *      + 30811261*a(n-11) + 100624740*a(n-12)
 *      - 269758235*a(n-13) - 288602304*a(n-14)
 *      + 1357891776*a(n-15) + 124420915*a(n-16)
 *      - 4267235783*a(n-17) + 21722225
 * The code below implements counting by dynamic programming over
 * row patterns of length 3, which is equivalent to iterating the
 * recurrence with integers of arbitrary size (seq_t).
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/smooth_sum.h"

/* Transition tables: next_cnt[C][r] is the number of rows s such that
 * the pair (r, s) has a 2×2 subblock sum equal to C (0 ≤ C ≤ 6).
 * next_state[C][r][k] stores the k‑th such s.
 */
static uint8_t next_cnt[7][27];
static uint8_t next_state[7][27][27];
static bool initialized = false;

/* Build the transition tables once. */
static void init_tables(void)
{
    if (initialized) return;
    for (int r = 0; r < 27; ++r) {
        int r0 = r / 9;          /* first column of row r */
        int r1 = (r / 3) % 3;    /* second column */
        int r2 = r % 3;          /* third column */
        for (int s = 0; s < 27; ++s) {
            int s0 = s / 9;
            int s1 = (s / 3) % 3;
            int s2 = s % 3;
            /* sums for the two 2×2 subblocks formed by rows r and s */
            int sum0 = smooth_sum(r0, r1, s0, s1);   /* columns 0‑1 */
            int sum1 = smooth_sum(r1, r2, s1, s2);   /* columns 1‑2 */
            if (sum0 == sum1) {
                int C = sum0;              /* 0 ≤ C ≤ 6 */
                int idx = next_cnt[C][r];
                next_state[C][r][idx] = (uint8_t)s;
                ++next_cnt[C][r];
            }
        }
    }
    initialized = true;
}

/* Main routine computing a(n). */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1 – ignore 0‑based queries */
        return (seq_t)0;

    init_tables();

    /* Two layers of DP: cur, nxt. */
    static seq_t dp[2][7][27];
    int cur = 0, nxt = 1;

    /* Initialise dp for length 0 (just one row, no 2×2 blocks yet). */
    for (int C = 0; C <= 6; ++C)
        for (int i = 0; i < 27; ++i)
            dp[cur][C][i] = (seq_t)1;

    seq_t total = 0;
    for (idx_t step = 1; step <= n; ++step) {
        /* zero next layer */
        for (int C = 0; C <= 6; ++C)
            for (int i = 0; i < 27; ++i)
                dp[nxt][C][i] = (seq_t)0;

        /* propagate one step (add another row) */
        for (int C = 0; C <= 6; ++C) {
            for (int i = 0; i < 27; ++i) {
                seq_t val = dp[cur][C][i];
                if (val == (seq_t)0) continue;
                uint8_t cnt = next_cnt[C][i];
                for (uint8_t k = 0; k < cnt; ++k) {
                    uint8_t j = next_state[C][i][k];
                    dp[nxt][C][j] += val;
                }
            }
        }

        /* accumulate total for this step */
        total = (seq_t)0;
        for (int C = 0; C <= 6; ++C)
            for (int i = 0; i < 27; ++i)
                total += dp[nxt][C][i];

        /* swap layers for next step */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    return total;
}
