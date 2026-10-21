/*
 * A208313  Number of nX7 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward neighbors.
 * https://oeis.org/A208313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 0..1 arrays with top‑left entry fixed to 0
 *            * (new values 0..1 introduced in row‑major order) and no element
 *            equal * to more than one of its immediate leftward or upward
 *            neighbors
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming over rows with 128 possible
 * row‑states (7 bits).  The first row is restricted to states with
 * the most‑significant bit (the top‑left entry) equal to 0.  For each
 * subsequent row we sum over all transitions that satisfy the local
 * constraint:
 *
 *   if (i>0 && j>0) a[i][j] must not equal both a[i-1][j] and a[i][j-1].
 *
 * This yields the correct values for all n≥1.  The DP runs in O(n·128²)
 * time, which is well within the limits for n up to about 10⁴.  All
 * arithmetic is done with the required seq_t type so that exact
 * results are produced for any supported width. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define STATES 128          /* 7 bits */

static bool transition[STATES][STATES];
static bool transition_ready = false;

static void build_transition(void) {
    for (int p = 0; p < STATES; ++p) {
        for (int q = 0; q < STATES; ++q) {
            bool ok = true;
            for (int j = 1; j < 7; ++j) {            /* j position in row */
                int cur  = (q >> j) & 1;
                int up   = (p >> j) & 1;
                int left = (q >> (j - 1)) & 1;
                if (cur == up && cur == left) {     /* equal to both neighbors */
                    ok = false;
                    break;
                }
            }
            transition[p][q] = ok;
        }
    }
}

seq_t a(idx_t n) {
    if (!transition_ready) {
        build_transition();
        transition_ready = true;
    }

    /* DP arrays for current and next row */
    seq_t dp[STATES] = {0};
    seq_t ndp[STATES] = {0};

    /* Initial row: top-left fixed to 0, all other bits free */
    for (int state = 0; state < STATES; ++state) {
        if ((state & 1) == 0)    /* least significant bit is leftmost column */
            dp[state] = 1;
    }

    if (n == 1) {
        /* Sum over all first‑row patterns */
        seq_t total = 0;
        for (int s = 0; s < STATES; ++s)
            total += dp[s];
        return total;
    }

    /* Iterate over remaining rows (n-1 transitions) */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < STATES; ++i) ndp[i] = 0;
        for (int p = 0; p < STATES; ++p) {
            if (dp[p] == 0) continue;
            for (int q = 0; q < STATES; ++q) {
                if (transition[p][q])
                    ndp[q] += dp[p];
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < STATES; ++i)
            dp[i] = ndp[i];
    }

    /* Sum over all final row patterns */
    seq_t total = 0;
    for (int s = 0; s < STATES; ++s)
        total += dp[s];
    return total;
}
