/*
 * A202467  Number of (n+2)X6 binary arrays with no more than two of any
 *          consecutive three bits set in any row or column.
 * https://oeis.org/A202467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A202467: Number of (n+2)×6 binary arrays with no more than two of any
 * consecutive three bits set in any row or column.
 * Computation is performed by a transfer–matrix dynamic programming
 * on the pair of successive rows. */

/* The size constants are compile‑time expressions.  The arrays
 * that depend on them are declared static inside the function so
 * that they are allocated exactly once.  All operations are performed
 * on the generic integer type `seq_t`, which may be an ordinary
 * unsigned integer or an arbitrary‑precision class defined by the build. */

#include <stdint.h>
#include <stdbool.h>

#define PATCNT 44                        /* number of 6‑bit row patterns without horizontal 111 */
#define STATES (PATCNT * PATCNT)         /* number of possible (previous two rows) states */

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* Static tables and vectors are re‑used for every call.             */
    /* ------------------------------------------------------------------ */

    static uint8_t rows[PATCNT];                 /* valid 6‑bit rows                         */
    static uint8_t transIdx[STATES][PATCNT];     /* allowed next rows for each state         */
    static uint8_t transCnt[STATES];             /* number of allowed transitions per state */
    static seq_t  dp_arr[STATES];                /* dynamic programming vector              */
    static seq_t  ndp_arr[STATES];
    static seq_t *dp  = dp_arr;
    static seq_t *ndp = ndp_arr;
    static bool init_done = false;

    /* ------------------------------------------------------------------ */
    /* One‑time initialization of the tables.                              */
    /* ------------------------------------------------------------------ */
    if (!init_done) {
        /* 1. Enumerate all allowed 6‑bit rows (no horizontal 111). */
        int r = 0;
        const int WIDTH = 6;
        for (int v = 0; v < (1 << WIDTH); ++v) {
            bool ok = true;
            for (int shift = 0; shift <= WIDTH - 3; ++shift)
                if (((v >> shift) & 0x7) == 0x7) { ok = false; break; }
            if (ok)
                rows[r++] = (uint8_t)v;
        }
        /* 2. Pre‑compute all transitions.  A transition from state
           (a,b) to (b,c) is allowed iff (rows[a] & rows[b] & rows[c]) == 0. */
        for (int a_idx = 0; a_idx < PATCNT; ++a_idx) {
            for (int b_idx = 0; b_idx < PATCNT; ++b_idx) {
                int state = a_idx * PATCNT + b_idx;
                uint8_t cnt = 0;
                uint8_t a_pat = rows[a_idx];
                uint8_t b_pat = rows[b_idx];
                for (int c_idx = 0; c_idx < PATCNT; ++c_idx) {
                    if ((a_pat & b_pat & rows[c_idx]) == 0)
                        transIdx[state][cnt++] = (uint8_t)c_idx;
                }
                transCnt[state] = cnt;
            }
        }
        init_done = true;
    }

    /* ------------------------------------------------------------------ */
    /* Special handling for very small matrix heights.                     */
    /* Height (h) = n + 2.                                                */
    /* ------------------------------------------------------------------ */
    idx_t h = n + 2;
    if (h == 1) return (seq_t)PATCNT;                      /* one row  */
    if (h == 2) return (seq_t)PATCNT * (seq_t)PATCNT;     /* two rows */

    /* ------------------------------------------------------------------ */
    /* Dynamic programming for height h >= 3.  We start with all
       possible pairs of first two rows.  For each subsequent row we
       add one transition.                                                */
    /* ------------------------------------------------------------------ */
    int steps = (int)(h - 2);                     /* number of transitions needed */

    /* Initialise dp: one way for each starting pair (a,b). */
    for (int i = 0; i < STATES; ++i) dp[i] = (seq_t)1;

    /* Perform the transitions step by step. */
    for (int s = 0; s < steps; ++s) {
        /* zero ndp */
        for (int i = 0; i < STATES; ++i) ndp[i] = 0;

        /* propagate all states */
        for (int state = 0; state < STATES; ++state) {
            seq_t val = dp[state];
            if (val == 0) continue;
            uint8_t a_idx = state / PATCNT;
            uint8_t b_idx = state % PATCNT;
            uint8_t cnt   = transCnt[state];
            for (uint8_t t = 0; t < cnt; ++t) {
                uint8_t c_idx = transIdx[state][t];
                int next_state = b_idx * PATCNT + c_idx;
                ndp[next_state] += val;
            }
        }
        /* swap dp and ndp for the next iteration */
        seq_t *tmp = dp; dp = ndp; ndp = tmp;
    }

    /* Sum over all final states to obtain the total number of matrices. */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i) total += dp[i];
    return total;
}
