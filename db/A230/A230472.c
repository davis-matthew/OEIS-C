/*
 * A230472  Number of 4Xn 0..2 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 3, and upper left element zero.
 * https://oeis.org/A230472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A230472 – Number of 4×n arrays with the adjacency condition.
    Computation by dynamic programming on columns.

    State: (pattern, unsat_mask)
        pattern   – 4 cells in the current column, values 0..2  (81 possibilities).
        unsat_mask – 4‑bit mask, bit r is 1 if column row r still needs a neighbour
                     with value (cell+1) mod 3 that hasn’t been found yet.
    Transition from state (p, u) to new pattern q is possible iff
        1. every still unsatisfied cell of the previous column (according to u)
           is satisfied by a neighbour of pattern q.
        2. all satisfied cells of the previous column become satisfied,
           otherwise the transition is rejected.
    After the transition the new unsat_mask for pattern q is set from the
    previous column and q; it tells which cells of q still need a neighbour
    on the right.  The final column must have unsat_mask == 0.
*/

#include <stdlib.h>
#include <stdbool.h>

#define PATTERNS 81          /* 3^4 */
#define UNSAT_BITS 4
#define STATES (PATTERNS * 16)   /* 81 * 2^4 */

static bool _initialized = false;
static int _colors[PATTERNS][UNSAT_BITS];
static int _trans[STATES][PATTERNS];
static seq_t _dpA[STATES];
static seq_t _dpB[STATES];
static seq_t *dp_cur = _dpA, *dp_next = _dpB;

/* Helper to precompute tables once. */
static void _init_tables(void)
{
    /* decode patterns into 4 digits. */
    for (int p = 0; p < PATTERNS; ++p) {
        int t = p;
        for (int r = 0; r < UNSAT_BITS; ++r) {
            _colors[p][r] = t % 3;
            t /= 3;
        }
    }

    /* transition table.  -1 indicates illegal transition. */
    for (int s = 0; s < STATES; ++s) {
        int pattern_a = s % PATTERNS;
        int unsat_a   = s / PATTERNS;
        int *col_a    = _colors[pattern_a];

        for (int p = 0; p < PATTERNS; ++p) {
            int *col_b = _colors[p];
            bool ok = true;

            /* 1. All still unsatisfied cells of column a must be satisfied by column b. */
            for (int r = 0; r < UNSAT_BITS; ++r) {
                bool sat_prev = !((unsat_a >> r) & 1);
                if (!sat_prev) {
                    int need = (col_a[r] + 1) % 3;
                    bool found = false;
                    if (col_b[r] == need) found = true;
                    if (r > 0 && col_b[r-1] == need) found = true;
                    if (r < 3 && col_b[r+1] == need) found = true;
                    if (!found) { ok = false; break; }
                }
            }
            if (!ok) { _trans[s][p] = -1; continue; }

            /* 2. Compute unsatisfied mask for column b after viewing column a. */
            int unsat_b = 0;
            for (int r = 0; r < UNSAT_BITS; ++r) {
                int need = (col_b[r] + 1) % 3;
                bool hasprev = false;
                if (col_a[r] == need) hasprev = true;
                if (r > 0 && col_a[r-1] == need) hasprev = true;
                if (r < 3 && col_a[r+1] == need) hasprev = true;
                if (!hasprev) unsat_b |= (1 << r);
            }

            int ns = p + PATTERNS * unsat_b;
            _trans[s][p] = ns;
        }
    }
    _initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (!_initialized) _init_tables();

    /* initialise DP for the first column (upper‑left entry must be 0). */
    for (int i = 0; i < STATES; ++i) dp_cur[i] = 0;
    for (int p = 0; p < PATTERNS; ++p) {
        if (_colors[p][0] == 0) {             /* upper‑left cell = 0 */
            int state = p + PATTERNS * 15;    /* all 4 cells unsatisfied yet */
            dp_cur[state] = 1;
        }
    }

    /* iterate columns 1 .. n-1 */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < STATES; ++i) dp_next[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            int *row = _trans[s];
            for (int p = 0; p < PATTERNS; ++p) {
                int ns = row[p];
                if (ns < 0) continue;
                dp_next[ns] += val;
            }
        }

        /* swap dp_next / dp_cur */
        seq_t *tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;
    }

    /* sum counts of states with unsatisfied mask == 0
       (state IDs 0 .. 80). */
    seq_t result = 0;
    for (int i = 0; i < PATTERNS; ++i)
        result += dp_cur[i];

    return result;
}
