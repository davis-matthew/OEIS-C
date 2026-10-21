/*
 * A207501  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary arrays avoiding the forbidden
 *            horizontal patterns 0 0 0 and 0 1 1 in each row, and the
 *            forbidden vertical patterns 0 0 1 and 1 0 0 in each column
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 25, u64 n <= 62, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: breadth‑first DP over columns.
   Initial values:
   a(1) = 9, a(2) = 81.  */
/* The sequence is computed by dynamic programming over columns; the
   implementation is O(n) in time and O(1) in space (constant 256 states).  */

#include <stdbool.h>

#define NUM_COLS 16               /* 4‑bit columns (0..15) */
#define NUM_STATES (NUM_COLS * NUM_COLS)  /* (prev1,prev2) pairs */

/* Pre‑calculated data, initialised once. */
static bool _allow_col[NUM_COLS];
static bool _trans[NUM_STATES][NUM_COLS];
static bool _trans_init = false;

/* Initialise the allowed columns and the transition table. */
static void _init_transitions(void)
{
    /* Determine vertically admissible 4‑bit columns. */
    for (int col = 0; col < NUM_COLS; ++col) {
        bool good = true;
        int b0 = (col >> 0) & 1;
        int b1 = (col >> 1) & 1;
        int b2 = (col >> 2) & 1;
        int b3 = (col >> 3) & 1;
        /* Top triple (rows 1‑3). */
        if ((b0 == 0 && b1 == 0 && b2 == 1) ||   /* 0 0 1 forbidden */
            (b0 == 1 && b1 == 0 && b2 == 0))     /* 1 0 0 forbidden */
            good = false;
        /* Bottom triple (rows 2‑4). */
        if ((b1 == 0 && b2 == 0 && b3 == 1) ||
            (b1 == 1 && b2 == 0 && b3 == 0))
            good = false;
        _allow_col[col] = good;
    }

    /* Pre‑compute transitions from state (prev1,prev2) to new column curr. */
    for (int s = 0; s < NUM_STATES; ++s) {
        int prev1 = s >> 4;      /* older column */
        int prev2 = s & 15;      /* newer column */
        for (int curr = 0; curr < NUM_COLS; ++curr) {
            if (!_allow_col[curr]) {
                _trans[s][curr] = false;
                continue;
            }
            bool ok = true;
            for (int r = 0; r < 4; ++r) {
                int p1 = (prev1 >> r) & 1;
                int p2 = (prev2 >> r) & 1;
                int c  = (curr   >> r) & 1;
                /* Horizontal forbidden patterns: 0 0 0 or 0 1 1. */
                if ((p1 == 0 && p2 == 0 && c == 0) ||
                    (p1 == 0 && p2 == 1 && c == 1)) {
                    ok = false;
                    break;
                }
            }
            _trans[s][curr] = ok;
        }
    }
    _trans_init = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Undefined for n == 0; return 0. */
    if (n == 0) return 0;

    /* Initialise transition data once. */
    if (! _trans_init) _init_transitions();

    /* Base cases. */
    if (n == 1) return 9;  /* number of admissible columns */
    if (n == 2) return 81; /* 9 × 9 */

    /* DP arrays: dp[prev1,prev2] = number of ways to reach this state. */
    static seq_t dp[NUM_STATES];
    static seq_t ndp[NUM_STATES];

    /* Reset dp to zero. */
    for (int i = 0; i < NUM_STATES; ++i) dp[i] = 0;

    /* Initialise dp for k = 2 (two columns). */
    for (int s = 0; s < NUM_STATES; ++s) {
        int p1 = s >> 4;
        int p2 = s & 15;
        dp[s] = (_allow_col[p1] && _allow_col[p2]) ? 1 : 0;
    }

    /* Expand column by column from 3 up to n. */
    for (idx_t col = 3; col <= n; ++col)
    {
        /* Clear ndp. */
        for (int i = 0; i < NUM_STATES; ++i) ndp[i] = 0;

        for (int s = 0; s < NUM_STATES; ++s) {
            seq_t ways = dp[s];
            if (ways == 0) continue;
            int prev1 = s >> 4;
            int prev2 = s & 15;
            int base = (prev1 << 4) | prev2;   /* same as s */
            for (int curr = 0; curr < NUM_COLS; ++curr) {
                if (!_trans[base][curr]) continue;
                int t = (prev2 << 4) | curr;
                ndp[t] += ways;
            }
        }

        /* Move ndp to dp for next iteration. */
        for (int i = 0; i < NUM_STATES; ++i) dp[i] = ndp[i];
    }

    /* Sum all states after processing n columns. */
    seq_t total = 0;
    for (int i = 0; i < NUM_STATES; ++i) total += dp[i];
    return total;
}
