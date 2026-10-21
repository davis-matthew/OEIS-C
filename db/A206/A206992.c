/*
 * A206992  Number of nX6 0..1 arrays avoiding 0 0 0 horizontally and 1 1 1
 *          vertically.
 * https://oeis.org/A206992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   dynamic programming over rows. a(n) = number of n×6 binary
 *            arrays with no horizontal 000 and no vertical 111. The DP state
 *            is the last two rows (a,b) and the transition to a new row c is
 *            allowed iff (rows[a] & rows[b] & rows[c]) == 0. Counts are
 *            accumulated for each n. The code memoises results u...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   has_horiz000
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/has_horiz000.h"

/* Parameters for the 6‑column problem */
#define ALLOWED_ROWS   44               /* number of 6‑bit rows without 000 */
#define STATE_COUNT    (ALLOWED_ROWS * ALLOWED_ROWS)
#define MAX_NEXT      ALLOWED_ROWS      /* maximum possible next rows per state */

/* Transition data: next state indices and counts for each state */
static uint8_t next_rows[STATE_COUNT][MAX_NEXT];
static uint8_t next_cnt[STATE_COUNT];

/* Allowed row patterns */
static uint8_t rows[ALLOWED_ROWS];

/* DP arrays for the last two rows */
static seq_t dp_prev[STATE_COUNT];
static seq_t dp_curr[STATE_COUNT];

/* Memoisation for a(n) values (maximum n we support is 10000). */
#define MAX_N 10000
static seq_t memo[MAX_N + 1];
static idx_t cached_max_n = 0;
static bool initialized = false;

/* Initialise static data once. */
static void init_once(void)
{
    /* Enumerate all allowed 6‑bit rows. */
    int r = 0;
    for (int pat = 0; pat < 64; ++pat) {
        if (!has_horiz000((uint8_t)pat)) {
            rows[r++] = (uint8_t)pat;
        }
    }

    /* Pre‑compute transitions from each pair (a,b). */
    for (int a = 0; a < ALLOWED_ROWS; ++a) {
        for (int b = 0; b < ALLOWED_ROWS; ++b) {
            int state = a * ALLOWED_ROWS + b;
            unsigned cnt = 0;
            uint8_t and_ab = rows[a] & rows[b];
            for (int c = 0; c < ALLOWED_ROWS; ++c) {
                if ((and_ab & rows[c]) == 0U) {
                    next_rows[state][cnt++] = (uint8_t)c;
                }
            }
            next_cnt[state] = (uint8_t)cnt;
        }
    }

    /* Initialise DP for n = 2: one way for each pair of rows. */
    for (int i = 0; i < STATE_COUNT; ++i) {
        dp_prev[i] = 1;
    }

    /* Initialise memoisation for offset 1 and 2. */
    memo[0] = 0;                       /* unused */
    memo[1] = (seq_t)ALLOWED_ROWS;     /* a(1) */
    memo[2] = (seq_t)ALLOWED_ROWS * (seq_t)ALLOWED_ROWS; /* a(2) */

    cached_max_n = 2;
    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!initialized) init_once();

    /* Handle trivial cases directly. */
    if (n <= cached_max_n)
        return memo[n];

    /* Extend DP up to the requested n. */
    for (idx_t step = cached_max_n + 1; step <= n; ++step) {

        /* Clear next DP array. */
        for (int i = 0; i < STATE_COUNT; ++i)
            dp_curr[i] = 0;

        /* Transition from dp_prev (state after step-1 rows)
           to dp_curr (state after step rows). */
        for (int state = 0; state < STATE_COUNT; ++state) {
            seq_t val = dp_prev[state];
            if (val == 0) continue;

            unsigned b = (unsigned)(state % ALLOWED_ROWS);   /* last row of previous pair */
            uint8_t cnt = next_cnt[state];
            uint8_t *list = next_rows[state];
            for (unsigned k = 0; k < cnt; ++k) {
                unsigned c = (unsigned)list[k];
                int next_state = b * ALLOWED_ROWS + c;
                dp_curr[next_state] += val;
            }
        }

        /* Sum all entries to get a(step). */
        seq_t sum = 0;
        for (int i = 0; i < STATE_COUNT; ++i)
            sum += dp_curr[i];

        memo[step] = sum;

        /* Prepare for next iteration. */
        for (int i = 0; i < STATE_COUNT; ++i)
            dp_prev[i] = dp_curr[i];
    }

    cached_max_n = n;
    return memo[n];
}
