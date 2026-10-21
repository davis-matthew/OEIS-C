/*
 * A239821  Number of 3Xn 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of the elements
 *          above it, modulo 4.
 * https://oeis.org/A239821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3×n arrays with entries in {0,1,2,3} * such
 *            that for every cell (r,c) the value a[r][c] is not congruent *
 *            to the sum of the elements to its left (mod 4) nor to 1 + the
 *            sum * of the elements above it (mod 4). The function computes
 *            this by * a DP with 64 states; see the implementa...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   idx_from_residues
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This file uses only language features common to C11 and C++17.
   It assumes that `seq_t` and `idx_t` are defined by the caller. */

#include <stdlib.h>   /* for size_t */
#include "../utils/idx_from_residues.h"

/* Number of states: each row has a residue 0..3 of the sum of elements
   to its left; 3 rows => 4^3 = 64 states. */
#define STATE_COUNT 64

/* Transition matrix: trans[s][t] = number of ways to go from state s
   to state t in one column.  Stored as unsigned char because the
   maximum value is small (≤27). */
static unsigned char trans[STATE_COUNT][STATE_COUNT];

/* Build the transition matrix once. */
static void init_transition_matrix(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    for (unsigned s = 0; s < STATE_COUNT; ++s) {
        unsigned l1 = (s >> 4) & 3;   /* current left residue for row 0 */
        unsigned l2 = (s >> 2) & 3;   /* for row 1    */
        unsigned l3 =  s        & 3;   /* for row 2    */

        for (unsigned a1 = 0; a1 < 4; ++a1) {
            /* row 0: cannot equal l1 (sum_left) or 1 (since sum_above = 0). */
            if (a1 == l1 || a1 == 1) continue;

            for (unsigned a2 = 0; a2 < 4; ++a2) {
                /* row 1: cannot equal l2 (sum_left) or 1+a1 (sum_above + 1). */
                if (a2 == l2 || a2 == (1 + a1) % 4) continue;

                for (unsigned a3 = 0; a3 < 4; ++a3) {
                    /* row 2: cannot equal l3 (sum_left) or 1+a1+a2 (sum_above+1). */
                    if (a3 == l3 || a3 == (1 + a1 + a2) % 4) continue;

                    /* New left‑sums after placing this column. */
                    unsigned nl1 = (l1 + a1) % 4;
                    unsigned nl2 = (l2 + a2) % 4;
                    unsigned nl3 = (l3 + a3) % 4;

                    unsigned t = idx_from_residues(nl1, nl2, nl3);
                    trans[s][t] += 1;
                }
            }
        }
    }
}

/* One‑dimensional DP arrays for counting column configurations. */
static seq_t dp_cur[STATE_COUNT];
static seq_t dp_next[STATE_COUNT];

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below offset is arbitrary */

    init_transition_matrix();

    /* Initialise DP for the first column. */
    for (unsigned i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 0;
    dp_cur[0] = 1;                 /* start state: all left sums are 0 */

    /* Process n columns. */
    for (idx_t col = 1; col <= n; ++col) {
        for (unsigned i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (unsigned s = 0; s < STATE_COUNT; ++s) {
            if (dp_cur[s] == 0) continue;
            for (unsigned t = 0; t < STATE_COUNT; ++t) {
                if (trans[s][t] != 0) {
                    dp_next[t] += dp_cur[s] * (seq_t)trans[s][t];
                }
            }
        }

        /* Swap dp_cur and dp_next for next iteration. */
        for (unsigned i = 0; i < STATE_COUNT; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum all ending states to obtain a(n). */
    seq_t total = 0;
    for (unsigned i = 0; i < STATE_COUNT; ++i) {
        total += dp_cur[i];
    }
    return total;
}
