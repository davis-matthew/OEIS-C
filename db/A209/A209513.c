/*
 * A209513  Number of (n+1) X 5 0..2 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to the number of
 *          counterclockwise edge increases.
 * https://oeis.org/A209513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A209513
 *  Number of (n+1)×5 arrays with entries 0..2 such that
 *  for every 2×2 subblock the numbers of clockwise edge increases
 *  equals the numbers of counter‑clockwise edge increases.
 *
 *  We compute the sequence by dynamic programming over the rows.
 *  Each row is a state of 5 base‑3 digits (3^5 = 243 states).
 *  A transition is allowed when all four 2×2 blocks between
 *  two consecutive rows satisfy the local equality.
 *  For a given n the number of arrays equals the number of
 *  sequences of length n+1 rows, i.e. the sum of all entries of
 *  dp after n transitions.
 *
 *  Complexity:   O(n · |S| · t)   where |S| = 243 and
 *  t is the average number of admissible transitions per state
 *  (much less than 243).  For n ≤ 10 000 this is well below the
 *  time limit on typical hardware. */

#include <stdint.h>

#define STATE_COUNT 243

static void precompute_transitions(uint8_t state_digits[STATE_COUNT][5],
                                   uint8_t trans_next[STATE_COUNT][STATE_COUNT],
                                   uint8_t trans_cnt[STATE_COUNT])
{
    /* encode all row states in base‑3 */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int x = s;
        for (int p = 4; p >= 0; --p) {
            state_digits[s][p] = x % 3;
            x /= 3;
        }
    }

    /* compute admissible transitions */
    for (int prev = 0; prev < STATE_COUNT; ++prev) {
        trans_cnt[prev] = 0;
        for (int next = 0; next < STATE_COUNT; ++next) {
            int ok = 1;
            for (int j = 0; j < 4; ++j) {
                int a = state_digits[prev][j];
                int b = state_digits[prev][j + 1];
                int c = state_digits[next][j];
                int d = state_digits[next][j + 1];

                int clk  = (a < b) + (b < d) + (d < c) + (c < a);
                int ccw  = (a < c) + (c < d) + (d < b) + (b < a);

                if (clk != ccw) { ok = 0; break; }
            }
            if (ok) {
                trans_next[prev][trans_cnt[prev]] = (uint8_t)next;
                trans_cnt[prev] += 1;
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* static tables reused across calls */
    static int initialized = 0;
    static uint8_t state_digits[STATE_COUNT][5];
    static uint8_t trans_next[STATE_COUNT][STATE_COUNT];
    static uint8_t trans_cnt[STATE_COUNT];

    if (!initialized) {
        precompute_transitions(state_digits, trans_next, trans_cnt);
        initialized = 1;
    }

    /* DP arrays */
    seq_t dp[STATE_COUNT];
    seq_t ndp[STATE_COUNT];

    /* initial row: any state is allowed */
    for (int s = 0; s < STATE_COUNT; ++s) dp[s] = (seq_t)1;

    /* perform n transitions (to obtain n+1 rows) */
    for (idx_t step = 0; step < n; ++step) {
        for (int s = 0; s < STATE_COUNT; ++s) ndp[s] = (seq_t)0;
        for (int prev = 0; prev < STATE_COUNT; ++prev) {
            if (dp[prev] == 0) continue;
            unsigned cnt = trans_cnt[prev];
            uint8_t *nexts = trans_next[prev];
            for (unsigned k = 0; k < cnt; ++k) {
                uint8_t next = nexts[k];
                ndp[next] += dp[prev];
            }
        }
        /* move to next step */
        for (int s = 0; s < STATE_COUNT; ++s) dp[s] = ndp[s];
    }

    /* sum all ways for the last row */
    seq_t ans = (seq_t)0;
    for (int s = 0; s < STATE_COUNT; ++s) ans += dp[s];
    return ans;
}
