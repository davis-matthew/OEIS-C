/*
 * A183332  Number of n X 4 binary arrays with each 1 adjacent to exactly two
 *          0's.
 * https://oeis.org/A183332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A183332: a(n) = number of n X 4 binary arrays with each 1 adjacent to exactly two 0’s.
   Computed by a linear‑time DP over column states. */
#include <stddef.h>
#include <stdbool.h>

/* sentinel value for a non‑existent column (used for the left side of the first column) */
static const uint8_t NO_COL = 16;          /* 4 rows → 16 possible bitmasks (0‑15) */

/* number of possible column bitmasks (0‑15) */
#define COLS 16
/* number of state rows (prev2 ∈ {0..15,NO_COL}) */
#define ROWS 17
/* total number of DP states: 17 × 16 = 272 */
#define STATES (ROWS * COLS)

/* check whether a column state (prev1) is valid given its left neighbour (prev2)
   and the right neighbour (cur).  For final validation cur is absent. */
static bool valid_transition(uint8_t prev2, uint8_t prev1, uint8_t cur, bool right_exists)
{
    /* iterate over the 4 rows (bits 0..3) */
    for (int r = 0; r < 4; ++r) {
        unsigned int bit1 = (prev1 >> r) & 1;
        if (!bit1) continue;          /* only 1’s are constrained */

        unsigned int zeros = 0;

        /* left neighbour */
        if (prev2 != NO_COL) {
            unsigned int b = (prev2 >> r) & 1;
            if (!b) ++zeros;
        }

        /* up neighbour */
        if (r > 0) {
            unsigned int b = (prev1 >> (r - 1)) & 1;
            if (!b) ++zeros;
        }

        /* down neighbour */
        if (r < 3) {
            unsigned int b = (prev1 >> (r + 1)) & 1;
            if (!b) ++zeros;
        }

        /* right neighbour (if it exists) */
        if (right_exists) {
            unsigned int b = (cur >> r) & 1;
            if (!b) ++zeros;
        }

        if (zeros != 2) return false; /* constraint violated */
    }
    return true; /* all 1’s satisfy the rule */
}

/* DP function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety, though n ≥ 1 by definition */

    /* static DP arrays of fixed size allow use both in C and C++ without constructors. */
    static seq_t dp[STATES];
    static seq_t ndp[STATES];

    /* initialise dp to zero */
    for (size_t i = 0; i < STATES; ++i) dp[i] = 0;

    /* initialise with the first column: prev2 = NO_COL (left missing) */
    for (uint8_t col = 0; col < COLS; ++col) {
        dp[NO_COL * COLS + col] = 1;
    }

    /* special case n == 1: just validate the sole column with both sides missing */
    if (n == 1) {
        seq_t result = 0;
        for (uint8_t p2 = 0; p2 < ROWS; ++p2) {
            for (uint8_t p1 = 0; p1 < COLS; ++p1) {
                size_t idx = p2 * COLS + p1;
                if (dp[idx] == 0) continue;
                if (valid_transition(p2, p1, 0, false)) result += dp[idx];
            }
        }
        return result;
    }

    /* DP over the remaining n‑1 columns */
    for (idx_t step = 2; step <= n; ++step) {
        /* zero ndp */
        for (size_t i = 0; i < STATES; ++i) ndp[i] = 0;

        for (uint8_t prev2 = 0; prev2 < ROWS; ++prev2) {
            for (uint8_t prev1 = 0; prev1 < COLS; ++prev1) {
                size_t idx = prev2 * COLS + prev1;
                seq_t curval = dp[idx];
                if (curval == 0) continue;

                for (uint8_t cur = 0; cur < COLS; ++cur) {
                    if (!valid_transition(prev2, prev1, cur, true))
                        continue;
                    size_t nidx = prev1 * COLS + cur;
                    ndp[nidx] += curval;
                }
            }
        }

        /* move ndp to dp for next iteration */
        for (size_t i = 0; i < STATES; ++i) dp[i] = ndp[i];
    }

    /* final validation for the last column (right neighbours missing) */
    seq_t result = 0;
    for (uint8_t prev2 = 0; prev2 < ROWS; ++prev2) {
        for (uint8_t prev1 = 0; prev1 < COLS; ++prev1) {
            size_t idx = prev2 * COLS + prev1;
            if (dp[idx] == 0) continue;
            if (valid_transition(prev2, prev1, 0, false))
                result += dp[idx];
        }
    }
    return result;
}
