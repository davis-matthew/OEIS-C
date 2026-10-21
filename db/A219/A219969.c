/*
 * A219969  Number of tilings of a 5 X n rectangle using straight (3 X 1)
 *          trominoes and 2 X 2 tiles.
 * https://oeis.org/A219969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A219969 - number of tilings of a 5×n rectangle
 *  using straight (3×1) trominoes and 2×2 squares.
 *
 *  We compute the values by dynamic programming over columns.
 *  A state is a pair of 5‑bit masks: the occupancy of the
 *  current column and the next column.  Since horizontal trominoes
 *  can span 3 columns we keep a third mask implicitly during the
 *  transition.  The transition from a state (cur,next) yields a
 *  new state (next',future) together with the number of ways.
 */

#include <stdint.h>

/* internal helper prototypes */
static void build_transitions(void);
static void dfs_rec(int row, int cur, int next, int fut, int *cnt);

/* transition tables  */
#define STATE_COUNT 1024   /* 2^(5+5) */
#define MAX_OUT 1024      /* max different output masks per state */

static int  trans_total[STATE_COUNT];
static int  trans_out[STATE_COUNT][MAX_OUT];
static int  trans_ways[STATE_COUNT][MAX_OUT];

/* compute the transition tables once */
static void build_transitions(void)
{
    for (int cur = 0; cur < 32; ++cur) {
        for (int next = 0; next < 32; ++next) {
            int state = cur | (next << 5);
            int cnt[MAX_OUT] = {0};
            dfs_rec(0, cur, next, 0, cnt);

            int t = 0;
            for (int out = 0; out < STATE_COUNT; ++out) {
                if (cnt[out]) {
                    trans_out[state][t] = out;
                    trans_ways[state][t] = cnt[out];
                    ++t;
                }
            }
            trans_total[state] = t;
        }
    }
}

/* recursive enumeration of all ways to fill the current column,
 * starting from masks cur (current column) and next (next column).
 * The third mask fut holds occupancy in the column after next
 * due to horizontal trominoes.  The array cnt stores how many
 * ways lead to each output state (next',future). */
static void dfs_rec(int row, int cur, int next, int fut, int *cnt)
{
    if (row == 5) {
        int out = next | (fut << 5);
        ++cnt[out];
        return;
    }
    if (cur & (1 << row)) {
        dfs_rec(row + 1, cur, next, fut, cnt);
        return;
    }

    /* vertical tromino (3×1) */
    if (row + 2 < 5 &&
        !(cur & (1 << (row + 1))) &&
        !(cur & (1 << (row + 2)))) {
        int cur2 = cur | (1 << row) | (1 << (row + 1)) | (1 << (row + 2));
        dfs_rec(row + 1, cur2, next, fut, cnt);
    }

    /* 2×2 square */
    if (row + 1 < 5 &&
        !(cur & (1 << row)) &&
        !(cur & (1 << (row + 1))) &&
        !(next & (1 << row)) &&
        !(next & (1 << (row + 1)))) {
        int cur2  = cur | (1 << row) | (1 << (row + 1));
        int next2 = next | (1 << row) | (1 << (row + 1));
        dfs_rec(row + 1, cur2, next2, fut, cnt);
    }

    /* horizontal tromino (1×3) */
    if (!(next & (1 << row)) && !(fut & (1 << row))) {
        int cur2  = cur | (1 << row);
        int next2 = next | (1 << row);
        int fut2  = fut  | (1 << row);
        dfs_rec(row + 1, cur2, next2, fut2, cnt);
    }
}

/* the required sequence function */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        build_transitions();
        initialized = 1;
    }

    static seq_t dp[STATE_COUNT];
    static seq_t ndp[STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < STATE_COUNT; ++i) ndp[i] = (seq_t)0;
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            int num = trans_total[s];
            for (int t = 0; t < num; ++t) {
                int out = trans_out[s][t];
                int ways = trans_ways[s][t];
                for (int k = 0; k < ways; ++k) ndp[out] += val;
            }
        }
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = ndp[i];
    }
    return dp[0];
}
