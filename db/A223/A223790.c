/*
 * A223790  Number of 3 X n 0..2 arrays with rows, diagonals and antidiagonals
 *          unimodal.
 * https://oeis.org/A223790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3×n arrays with entries 0..2 such that every *
 *            row, main diagonal and anti‑diagonal is unimodal. The
 *            implementation * uses dynamic programming over pairs of
 *            consecutive columns and row * direction flags
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 97, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

enum {
    COLS = 27,                     /* 3^3 possible columns */
    FLAGS = 8,                     /* 2^3 row direction flags */
    STATE_COUNT = COLS * COLS * FLAGS,   /* 27*27*8 = 5832 */
    TRANS_MAX = COLS                /* at most 27 outgoing transitions */
};

static int col_digits[COLS][3];          /* column -> (r0,r1,r2) */
static int trans_next[STATE_COUNT][TRANS_MAX];
static int trans_count[STATE_COUNT];

static void init_precalc(void)
{
    static bool ready = false;
    if (ready) return;

    /* Build digit table */
    for (int v = 0; v < COLS; ++v) {
        int x = v;
        col_digits[v][0] = x % 3;
        x /= 3;
        col_digits[v][1] = x % 3;
        x /= 3;
        col_digits[v][2] = x;
    }

    /* Build state transitions */
    for (int state = 0; state < STATE_COUNT; ++state) {
        int flags   = state % FLAGS;
        int tmplst  = state / FLAGS;
        int col1    = tmplst % COLS;    /* previous column */
        int col0    = tmplst / COLS;    /* two columns back */

        int idx = 0;
        for (int new_col = 0; new_col < COLS; ++new_col) {
            int new_flags = flags;
            bool bad = false;

            /* Row unimodality */
            for (int r = 0; r < 3; ++r) {
                int prev_val = col_digits[col1][r];
                int cur_val  = col_digits[new_col][r];
                if ((flags >> r) & 1) {          /* already decreasing */
                    if (cur_val > prev_val) { bad = true; break; }
                } else {                          /* still nondecreasing */
                    if (cur_val < prev_val) new_flags |= (1 << r);
                }
            }
            if (bad) continue;

            /* Main diagonal valley check: (0,0)->(1,1)->(2,2) */
            int a = col_digits[col0][0];
            int b = col_digits[col1][1];
            int c = col_digits[new_col][2];
            if (a > b && b < c) continue;      /* valley forbidden */

            /* Anti‑diagonal valley check: (2,0)->(1,1)->(0,2) */
            a = col_digits[col0][2];
            b = col_digits[col1][1];
            c = col_digits[new_col][0];
            if (a > b && b < c) continue;      /* valley forbidden */

            /* Accept this transition */
            int new_state = ((col1 * COLS + new_col) * FLAGS) | new_flags;
            trans_next[state][idx++] = new_state;
        }
        trans_count[state] = idx;
    }
    ready = true;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)27;
    if (n == 2) return (seq_t)729;

    init_precalc();

    static seq_t dp[STATE_COUNT];
    static seq_t dp_next[STATE_COUNT];

    /* Reset dp array */
    memset(dp, 0, sizeof(dp));

    /* Initial states after first two columns */
    for (int col0 = 0; col0 < COLS; ++col0) {
        for (int col1 = 0; col1 < COLS; ++col1) {
            int flags = 0;
            for (int r = 0; r < 3; ++r) {
                if (col_digits[col0][r] > col_digits[col1][r]) flags |= (1 << r);
            }
            int state = ((col0 * COLS + col1) * FLAGS) | flags;
            dp[state] = (seq_t)1;
        }
    }

    /* Process remaining columns */
    for (idx_t step = 0; step < n - 2; ++step) {
        memset(dp_next, 0, sizeof(dp_next));

        for (int state = 0; state < STATE_COUNT; ++state) {
            seq_t cur = dp[state];
            if (cur == 0) continue;
            int cnt = trans_count[state];
            for (int k = 0; k < cnt; ++k) {
                int nxt = trans_next[state][k];
                dp_next[nxt] += cur;
            }
        }
        /* copy next to dp */
        memcpy(dp, dp_next, sizeof(dp));
    }

    /* Sum all possible final states */
    seq_t ans = 0;
    for (int state = 0; state < STATE_COUNT; ++state) ans += dp[state];
    return ans;
}
