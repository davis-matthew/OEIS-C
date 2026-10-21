/*
 * A202610  Number of (n+2)X4 binary arrays avoiding patterns 001 and 010 in
 *          rows and columns.
 * https://oeis.org/A202610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   forbidden_triple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A202610: Number of (n+2)×4 binary arrays avoiding 001 and 010
   in rows and columns.
   Computation by transfer‑matrix DP over 81 states (pairs of consecutive
   rows).  The algorithm is independent of any hard‑coded table
   and works for any n ≥ 1.  */
#include <stdint.h>
#include "../utils/forbidden_triple.h"

/* Constants */
enum {
    ALLOWED_ROWS_COUNT = 9,            /* number of 4‑bit row patterns that avoid 001 and 010 horizontally */
    STATE_COUNT = ALLOWED_ROWS_COUNT * ALLOWED_ROWS_COUNT
};

/* All horizontal‑valid 4‑bit rows (in integer form 0–15) */
static const uint8_t ALLOWED_ROWS[ALLOWED_ROWS_COUNT] = {
    0x0, /* 0000 */
    0x6, /* 0110 */
    0x7, /* 0111 */
    0x8, /* 1000 */
    0xB, /* 1011 */
    0xC, /* 1100 */
    0xD, /* 1101 */
    0xE, /* 1110 */
    0xF  /* 1111 */
};

/* Transition tables: for each pair (prev,cur) we list admissible next rows. */
static int TRANS_COUNT[STATE_COUNT];
static int TRANS_NEXT[STATE_COUNT][ALLOWED_ROWS_COUNT];

/* Build the transition tables once */
static void build_transition_table(void)
{
    for (int p = 0; p < ALLOWED_ROWS_COUNT; ++p) {
        for (int c = 0; c < ALLOWED_ROWS_COUNT; ++c) {
            int idx = p * ALLOWED_ROWS_COUNT + c;
            int cnt = 0;
            for (int n = 0; n < ALLOWED_ROWS_COUNT; ++n) {
                int ok = 1;
                for (int j = 0; j < 4; ++j) {
                    int a = (ALLOWED_ROWS[p] >> j) & 1;
                    int b = (ALLOWED_ROWS[c] >> j) & 1;
                    int d = (ALLOWED_ROWS[n] >> j) & 1;
                    if (forbidden_triple(a, b, d)) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    TRANS_NEXT[idx][cnt++] = n;
                }
            }
            TRANS_COUNT[idx] = cnt;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* number of rows in the matrix = n + 2 */
    idx_t m = n + 2;
    if (m < 3)                     /* safety – should not happen for n >= 1 */
        return 0;

    /* initialise transition table once */
    static int initialized = 0;
    if (!initialized) {
        build_transition_table();
        initialized = 1;
    }

    /* DP arrays: dp[state] – ways to reach this state after current row
       (state = prev * 9 + cur).  ndp used for the next step. */
    seq_t dp[STATE_COUNT] = {0};
    seq_t ndp[STATE_COUNT] = {0};

    /* Initialisation: any pair of allowed rows satisfies horizontal
       constraints for the first two rows. */
    for (int p = 0; p < ALLOWED_ROWS_COUNT; ++p) {
        for (int c = 0; c < ALLOWED_ROWS_COUNT; ++c) {
            int state = p * ALLOWED_ROWS_COUNT + c;
            dp[state] = 1;
        }
    }

    /* Process remaining rows (from row 3 to m). */
    for (idx_t step = 3; step <= m; ++step) {
        /* reset ndp */
        for (int i = 0; i < STATE_COUNT; ++i)
            ndp[i] = 0;

        for (int st = 0; st < STATE_COUNT; ++st) {
            seq_t ways = dp[st];
            if (ways == 0) continue;
            int prev = st / ALLOWED_ROWS_COUNT;
            int cur  = st % ALLOWED_ROWS_COUNT;
            int tcnt = TRANS_COUNT[st];
            for (int k = 0; k < tcnt; ++k) {
                int nxt = TRANS_NEXT[st][k];
                int newstate = cur * ALLOWED_ROWS_COUNT + nxt;
                ndp[newstate] += ways;
            }
        }

        /* swap dp and ndp for next iteration */
        for (int i = 0; i < STATE_COUNT; ++i)
            dp[i] = ndp[i];
    }

    /* Sum all final states to obtain the total number of arrays. */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i)
        total += dp[i];
    return total;
}
