/*
 * A208440  Number of nX3 0..2 arrays with no element equal the average of
 *          immediate neighbors vertically above and horizontally left of it.
 * https://oeis.org/A208440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of n×3 arrays over {0,1,2} in which no element equals the
 *            * average of the immediately above and left neighbors (when both
 *            * neighbors exist). For a missing neighbor the element must
 *            simply * differ from the existing adjacent neighbor. This is a
 *            linear DP * over 27 states
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* --------------------------------------------------------------------
     *  States: all 3‑tuples of values 0..2 → 27 possibilities.
     *  Transition matrix T[i][j] = 1 iff a row having state i may be
     *  followed by a row having state j.
     * -------------------------------------------------------------------- */
    enum { STATE_COUNT = 27 };
    static unsigned char states[STATE_COUNT][3];
    static unsigned char trans[STATE_COUNT][STATE_COUNT];
    static int initialized = 0;

    if (!initialized) {
        /* encode all states */
        for (int k = 0; k < STATE_COUNT; ++k) {
            states[k][0] = k / 9;           /* most significant trit */
            states[k][1] = (k / 3) % 3;     /* middle trit   */
            states[k][2] = k % 3;           /* least  trit   */
        }

        /* build transition matrix for rows ≥ 2 */
        for (int i = 0; i < STATE_COUNT; ++i) {
            unsigned char a0 = states[i][0];
            unsigned char a1 = states[i][1];
            unsigned char a2 = states[i][2];
            for (int j = 0; j < STATE_COUNT; ++j) {
                unsigned char b0 = states[j][0];
                unsigned char b1 = states[j][1];
                unsigned char b2 = states[j][2];

                /* vertical constraint for leftmost column */
                if (b0 == a0) { trans[i][j] = 0; continue; }

                /* middle column: compare with average of above and left */
                if ( ((a1 + b0) & 1) == 0 ) {
                    unsigned char avg = (a1 + b0) / 2;
                    if (b1 == avg) { trans[i][j] = 0; continue; }
                }

                /* right column: compare with average of above and left */
                if ( ((a2 + b1) & 1) == 0 ) {
                    unsigned char avg = (a2 + b1) / 2;
                    if (b2 == avg) { trans[i][j] = 0; continue; }
                }

                trans[i][j] = 1;
            }
        }
        initialized = 1;
    }

    /* --------------------------------------------------------------------
     *  DP over rows.
     *  dp_prev[s] = number of ways to fill the rows processed so far
     *  and end in state s.
     * -------------------------------------------------------------------- */
    seq_t dp_prev[STATE_COUNT];
    seq_t dp_curr[STATE_COUNT];

    /* initialise to zero */
    for (int i = 0; i < STATE_COUNT; ++i) dp_prev[i] = 0;

    /* first row: only horizontal adjacency restriction *
     * result = 0 unless values in the row are pairwise unequal
     * (i.e. v1 != v0 and v2 != v1).  These are the only allowed states.
     */
    for (int s = 0; s < STATE_COUNT; ++s) {
        if (states[s][1] != states[s][0] && states[s][2] != states[s][1]) {
            dp_prev[s] = 1;
        }
    }

    /* if only one row requested, sum and return */
    if (n == 1) {
        seq_t total = 0;
        for (int s = 0; s < STATE_COUNT; ++s) total += dp_prev[s];
        return total;
    }

    /* process rows 2..n */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero current */
        for (int j = 0; j < STATE_COUNT; ++j) dp_curr[j] = 0;

        for (int i = 0; i < STATE_COUNT; ++i) {
            if (dp_prev[i] == 0) continue;
            for (int j = 0; j < STATE_COUNT; ++j) {
                if (!trans[i][j]) continue;
                dp_curr[j] += dp_prev[i];
            }
        }

        /* copy current to previous for next iteration */
        for (int j = 0; j < STATE_COUNT; ++j) dp_prev[j] = dp_curr[j];
    }

    /* sum all ending states to get total number of arrays */
    seq_t result = 0;
    for (int s = 0; s < STATE_COUNT; ++s) result += dp_prev[s];
    return result;
}
