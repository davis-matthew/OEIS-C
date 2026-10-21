/*
 * A089937  Number of 6 X n matrices with entries {0,1} without adjacent 0's
 *          in any row or column. 6th row of A089934.
 * https://oeis.org/A089937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts the number of 6×n binary matrices with no two adjacent 0’s.
   It equals the number of independent vertex sets in the grid graph P₆×Pₙ.
   The DP works over the 21 column‑state masks (no vertical adjacency) and
   permits a transition only when the two masks are disjoint (no horizontal adjacency). */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    /* Precompute the 21 allowed column masks and the transition table */
    static bool init = false;
    static int  states[21];
    static bool trans[21][21];
    static int  state_cnt = 0;

    if (!init) {
        /* generate masks with no two consecutive 1’s (vertical 0‑adjacent forbidden) */
        for (int m = 0; m < 64; ++m) {
            bool ok = true;
            for (int i = 0; i < 5; ++i)        /* check adjacent bits */
                if (((m >> i) & 1) && ((m >> (i + 1)) & 1)) {
                    ok = false;
                    break;
                }
            if (ok) {
                states[state_cnt++] = m;
            }
        }
        /* compute transition table: masks must be disjoint to avoid horizontal 0‑adjacency */
        for (int s = 0; s < state_cnt; ++s)
            for (int t = 0; t < state_cnt; ++t)
                trans[s][t] = ((states[s] & states[t]) == 0);
        init = true;
    }

    /* DP arrays for two consecutive columns */
    seq_t dp_prev[21], dp_next[21];
    /* first column: one way for each state */
    for (int s = 0; s < state_cnt; ++s) dp_prev[s] = 1;

    /* iterate over remaining columns */
    for (idx_t col = 2; col <= n; ++col) {
        for (int t = 0; t < state_cnt; ++t) dp_next[t] = 0;
        for (int s = 0; s < state_cnt; ++s) {
            seq_t val = dp_prev[s];
            if (val == 0) continue;
            for (int t = 0; t < state_cnt; ++t) {
                if (trans[s][t]) dp_next[t] += val;
            }
        }
        /* move to next column */
        for (int s = 0; s < state_cnt; ++s) dp_prev[s] = dp_next[s];
    }

    /* sum over all states gives a(n) */
    seq_t total = 0;
    for (int s = 0; s < state_cnt; ++s) total += dp_prev[s];
    return total;
}
