/*
 * A207466  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays avoiding horizontally * the
 *            patterns 000 and 011 and vertically the patterns 010 and 101. *
 *            Computed by dynamic programming on pairs of rows
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define COLS 7
#define VROWS 25
#define STATE_COUNT (VROWS * VROWS)
#define MAX_TRANS VROWS   /* maximum outgoing transitions from a state */

/* Test whether a 7‑bit pattern satisfies the horizontal constraints. */
static bool valid_row(uint16_t r)
{
    for (int i = 0; i <= COLS - 3; ++i) {
        int b0 = (r >> i) & 1;
        int b1 = (r >> (i + 1)) & 1;
        int b2 = (r >> (i + 2)) & 1;
        if ((b0 == 0 && b1 == 0 && b2 == 0) ||
            (b0 == 0 && b1 == 1 && b2 == 1))
            return false;
    }
    return true;
}

/* Test vertical constraint for three consecutive rows. */
static bool vertical_ok(uint16_t a, uint16_t b, uint16_t c)
{
    for (int j = 0; j < COLS; ++j) {
        int aa = (a >> j) & 1;
        int bb = (b >> j) & 1;
        int cc = (c >> j) & 1;
        if ((aa == 0 && bb == 1 && cc == 0) ||
            (aa == 1 && bb == 0 && cc == 1))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Static data that is computed only once. */
    static bool initialized = false;
    static uint16_t valid_rows[VROWS];
    static int next_state[STATE_COUNT][MAX_TRANS];
    static int next_count[STATE_COUNT];
    static seq_t dp_prev[STATE_COUNT];
    static seq_t dp_curr[STATE_COUNT];
    static int total_valid = 0;

    if (!initialized) {
        /* Find all rows that satisfy horizontal rules. */
        for (int pat = 0; pat < (1 << COLS); ++pat) {
            if (valid_row((uint16_t)pat)) {
                valid_rows[total_valid++] = (uint16_t)pat;
            }
        }
        /* Sanity check: should be exactly VROWS rows. */
        if (total_valid != VROWS) {
            return (seq_t)0; /* should never happen */
        }

        /* Pre‑compute transitions between states. */
        for (int iPrevPrev = 0; iPrevPrev < VROWS; ++iPrevPrev) {
            for (int iPrev = 0; iPrev < VROWS; ++iPrev) {
                int s = iPrevPrev * VROWS + iPrev;
                int cnt = 0;
                uint16_t rPrevPrev = valid_rows[iPrevPrev];
                uint16_t rPrev = valid_rows[iPrev];
                for (int iNew = 0; iNew < VROWS; ++iNew) {
                    uint16_t rNew = valid_rows[iNew];
                    if (vertical_ok(rPrevPrev, rPrev, rNew)) {
                        next_state[s][cnt++] = iPrev * VROWS + iNew;
                    }
                }
                next_count[s] = cnt;
            }
        }

        initialized = true;
    }

    /* Handle n = 1 and n = 2 directly. */
    if (n == (idx_t)1) {
        return (seq_t)VROWS;
    }
    if (n == (idx_t)2) {
        return (seq_t)VROWS * (seq_t)VROWS;
    }

    /* n >= 3: run DP.  Start with all ordered pairs of valid rows. */
    for (int i = 0; i < STATE_COUNT; ++i) {
        dp_prev[i] = (seq_t)0;
    }
    for (int i = 0; i < VROWS; ++i) {
        for (int j = 0; j < VROWS; ++j) {
            int idx = i * VROWS + j;
            dp_prev[idx] = (seq_t)1;
        }
    }

    /* Iterate rows 3..n. */
    for (idx_t step = (idx_t)3; step <= n; ++step) {
        /* Clear current dp. */
        for (int i = 0; i < STATE_COUNT; ++i) {
            dp_curr[i] = (seq_t)0;
        }
        /* Transition from each state. */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t ways = dp_prev[s];
            if (ways == (seq_t)0)
                continue;
            int c = next_count[s];
            for (int t = 0; t < c; ++t) {
                int ns = next_state[s][t];
                dp_curr[ns] += ways;
            }
        }
        /* Prepare for next step. */
        for (int i = 0; i < STATE_COUNT; ++i) {
            dp_prev[i] = dp_curr[i];
        }
    }

    /* Sum all possibilities for the final row. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i) {
        result += dp_prev[i];
    }
    return result;
}
