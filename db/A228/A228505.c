/*
 * A228505  Number of nX7 binary arrays with top left value 1 and no two ones
 *          adjacent horizontally, vertically, diagonally or antidiagonally.
 * https://oeis.org/A228505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over 7‑bit row states with
   no adjacent ones horizontally, vertically, diagonally, or
   antidiagonally.  The top left bit must be 1.  The algorithm
   enumerates all 34 valid row masks and precomputes a transition
   table that ensures forbidden positions are avoided.  For
   any n ≥ 1 it returns the number of n×7 binary arrays
   satisfying the constraints. */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Rigidly limited to 7 columns → 34 valid row masks. */
    enum { MAX_STATES = 34 };

    /* Cached global data: states, count, and transitions. */
    static bool initialized = false;
    static idx_t  states[MAX_STATES];
    static size_t state_count = 0;
    static bool   trans[MAX_STATES][MAX_STATES];

    /* Initialise once. */
    if (!initialized) {
        /* Enumerate all 7‑bit masks without horizontal adjacency. */
        for (idx_t m = 0; m < ((idx_t)1 << 7); ++m) {
            if ((m & (m << 1)) == 0) {           /* no adjacent 1s */
                states[state_count++] = m;
            }
        }

        /* Pre‑compute admissible transitions. */
        for (size_t i = 0; i < state_count; ++i) {
            for (size_t j = 0; j < state_count; ++j) {
                idx_t s1 = states[i];
                idx_t s2 = states[j];

                /* Positions forbidden in the next row: same column,
                   diagonally up/down left/right. */
                idx_t forbidden = s1 |                 /* vertical   */
                                  ((s1 << 1) & 0x7FULL)/* anti‑ diagonal */
                                  | (s1 >> 1);        /* diagonal   */
                trans[i][j] = (forbidden & s2) == 0;
            }
        }
        initialized = true;
    }

    /* Out of sequence: return zero. */
    if (n == 0) return 0;

    /* DP arrays on the stack. */
    seq_t dp[MAX_STATES];
    seq_t newdp[MAX_STATES];

    /* Initial row: top‑left value must be 1. */
    for (size_t i = 0; i < state_count; ++i)
        dp[i] = (states[i] & 1) ? 1 : 0;

    /* Process remaining rows. */
    for (idx_t row = 2; row <= n; ++row) {
        for (size_t i = 0; i < state_count; ++i) newdp[i] = 0;

        for (size_t i = 0; i < state_count; ++i) {
            if (dp[i] == 0) continue;
            seq_t val = dp[i];
            for (size_t j = 0; j < state_count; ++j) {
                if (trans[i][j]) newdp[j] += val;
            }
        }

        /* Copy to dp for next iteration. */
        for (size_t i = 0; i < state_count; ++i) dp[i] = newdp[i];
    }

    /* Sum all valid last‑row states. */
    seq_t result = 0;
    for (size_t i = 0; i < state_count; ++i) result += dp[i];
    return result;
}
