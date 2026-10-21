/*
 * A205514  Number of (n+1)X3 0..2 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock differing from each horizontal or
 *          vertical neighbor.
 * https://oeis.org/A205514
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

/* A205514:
   Number of (n+1)×3 arrays with values 0..2 such that for every 2×2 subblock
   the number of clockwise edge‑increases differs from each of its horizontal
   and vertical neighbors.
   Computed by a transfer‑matrix dynamic programming over columns. */

#include <stddef.h>
#include <stdint.h>

/* Section constants */
#define PATTERN_COUNT 27         /* 3^3 possible column patterns */
#define STATE_COUNT   (PATTERN_COUNT * 25) /* pattern × 5 × 5 (two counts 0..4) */

/* Pre‑computed data */
static int pattern_digits[PATTERN_COUNT][3];
static int trans_next[STATE_COUNT][PATTERN_COUNT]; /* at most 27 next states per state */
static unsigned char trans_next_count[STATE_COUNT];

/* Helper to generate the two clockwise‑increase counts for a 2×2 block */
static inline int subblock_cw_inc(int pat_prev, int pat_new, int row) {
    /* row is 0 or 1 */
    int a0 = pattern_digits[pat_prev][row];
    int a1 = pattern_digits[pat_prev][row + 1];
    int b0 = pattern_digits[pat_new][row];
    int b1 = pattern_digits[pat_new][row + 1];
    int cnt = 0;
    if (a0 < b0) ++cnt;
    if (b0 < b1) ++cnt;
    if (b1 < a1) ++cnt;
    if (a1 < a0) ++cnt;
    return cnt;
}

/* One‑time static initialisation of table data */
static void init_tables(void) {
    /* digits for each pattern */
    for (int pat = 0; pat < PATTERN_COUNT; ++pat) {
        int v = pat;
        for (int r = 2; r >= 0; --r) {
            pattern_digits[pat][r] = v % 3;
            v /= 3;
        }
    }

    /* transition table */
    for (int st = 0; st < STATE_COUNT; ++st) {
        int pat_prev = st / 25;
        int cnt_prev0 = (st / 5) % 5;
        int cnt_prev1 = st % 5;

        unsigned char cnt = 0;
        for (int pat_new = 0; pat_new < PATTERN_COUNT; ++pat_new) {
            int cnt_new0 = subblock_cw_inc(pat_prev, pat_new, 0);
            int cnt_new1 = subblock_cw_inc(pat_prev, pat_new, 1);

            /* constraints */
            if (cnt_new0 == cnt_prev0 || cnt_new1 == cnt_prev1 || cnt_new0 == cnt_new1)
                continue;

            int new_state = pat_new * 25 + cnt_new0 * 5 + cnt_new1;
            trans_next[st][cnt++] = new_state;
        }
        trans_next_count[st] = cnt;
    }
}

/* The required sequence function */
seq_t a(idx_t n)
{
    static int tables_ready = 0;
    if (!tables_ready) {
        init_tables();
        tables_ready = 1;
    }

    /* DP state arrays */
    static seq_t dp[STATE_COUNT];
    static seq_t dp_next[STATE_COUNT];
    size_t i, j;

    /* Initialise all dp values to zero */
    for (i = 0; i < STATE_COUNT; ++i) dp[i] = 0;

    /* Initialise using all possible first two columns (pair (p0,p1)) */
    for (int p0 = 0; p0 < PATTERN_COUNT; ++p0) {
        for (int p1 = 0; p1 < PATTERN_COUNT; ++p1) {
            int cnt0 = subblock_cw_inc(p0, p1, 0);
            int cnt1 = subblock_cw_inc(p0, p1, 1);
            if (cnt0 == cnt1) continue;          /* vertical neighbour constraint */
            int state_id = p1 * 25 + cnt0 * 5 + cnt1;
            dp[state_id] += (seq_t)1;
        }
    }

    /* If only one subblock (n == 1) we are already done */
    if (n == 1)
        goto finish;

    /* Perform remaining n-1 transitions */
    for (size_t step = 1; step < (size_t)n; ++step) {
        /* zero dp_next */
        for (i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (i = 0; i < STATE_COUNT; ++i) {
            seq_t cur_val = dp[i];
            if (!cur_val) continue;
            unsigned char trcnt = trans_next_count[i];
            for (j = 0; j < trcnt; ++j) {
                int nxt = trans_next[i][j];
                dp_next[nxt] += cur_val;
            }
        }
        /* swap dp and dp_next */
        for (i = 0; i < STATE_COUNT; ++i) dp[i] = dp_next[i];
    }

finish:
    /* sum over all end states */
    seq_t result = 0;
    for (i = 0; i < STATE_COUNT; ++i)
        result += dp[i];

    return result;
}
