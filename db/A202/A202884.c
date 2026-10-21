/*
 * A202884  Number of nX3 0..2 arrays with every nonzero element less than or
 *          equal to some horizontal or vertical neighbor.
 * https://oeis.org/A202884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2} where every * nonzero
 *            element is ≤ at least one of its horizontal or vertical *
 *            neighbors. Sequence definition uses the property that * each
 *            nonzero cell must have a neighboring cell (in the same * row or
 *            above/below) with value ≥ itself. The function belo...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Helper tables for the DP.                                            */
/* --------------------------------------------------------------------- */

/* Pattern index 0..26 encodes the 3 values of a row: */
static int patVals[27][3];

/* For a given state (previous pattern and pending mask) we precompute
 * all possible next states and count them.  A state consists of
 * 27 possible patterns * 8 possible pending masks = 216 states. */
static int transTo[216][27];
static unsigned char transCnt[216];

/* --------------------------------------------------------------------- */
/* Initialise the helper tables once.                                    */
/* --------------------------------------------------------------------- */
static void init_tables(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Fill the pattern table. */
    for (int p = 0; p < 27; ++p) {
        patVals[p][0] = p % 3;
        patVals[p][1] = (p / 3) % 3;
        patVals[p][2] = p / 9;
    }

    /* Build transition tables. */
    for (int pst = 0; pst < 216; ++pst) {
        int prev_pat   = pst / 8;
        int prev_mask  = pst % 8;
        int prev_vals[3] = { patVals[prev_pat][0],
                             patVals[prev_pat][1],
                             patVals[prev_pat][2] };

        unsigned char k = 0;
        for (int cur_pat = 0; cur_pat < 27; ++cur_pat) {
            int cur_vals[3] = { patVals[cur_pat][0],
                                patVals[cur_pat][1],
                                patVals[cur_pat][2] };

            /* Check that every pending previous cell is satisfied
               by the current cell (vertical neighbor). */
            bool ok = true;
            for (int j = 0; j < 3; ++j) {
                if ((prev_mask >> j) & 1) {
                    if (cur_vals[j] < prev_vals[j]) { ok = false; break; }
                }
            }
            if (!ok) continue;

            /* Compute pending mask for the current row. */
            int cur_mask = 0;
            for (int j = 0; j < 3; ++j) {
                if (cur_vals[j] == 0) continue;
                bool sat = false;
                /* vertical neighbor (above) */
                if (prev_vals[j] >= cur_vals[j]) sat = true;
                /* horizontal left neighbor */
                if (!sat && j > 0 && cur_vals[j-1] >= cur_vals[j]) sat = true;
                /* horizontal right neighbor */
                if (!sat && j < 2 && cur_vals[j+1] >= cur_vals[j]) sat = true;
                if (!sat) cur_mask |= (1 << j);
            }

            int next_state = cur_pat * 8 + cur_mask;
            transTo[pst][k++] = next_state;
        }
        transCnt[pst] = k;
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function.                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_tables();  /* ensure tables are ready */

    seq_t dp_cur[216]  = {0};
    seq_t dp_next[216] = {0};

    /* ---------- Base case: first row ---------- */
    for (int pat = 0; pat < 27; ++pat) {
        int mask = 0;
        int vals[3] = { patVals[pat][0], patVals[pat][1], patVals[pat][2] };
        for (int j = 0; j < 3; ++j) {
            if (vals[j] == 0) continue;
            bool sat = false;
            /* horizontal left neighbor */
            if (j > 0 && vals[j-1] >= vals[j]) sat = true;
            /* horizontal right neighbor */
            if (!sat && j < 2 && vals[j+1] >= vals[j]) sat = true;
            if (!sat) mask |= (1 << j);
        }
        int state = pat * 8 + mask;
        dp_cur[state] = (seq_t)1;
    }

    /* ---------- DP for subsequent rows ---------- */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 216; ++i) dp_next[i] = (seq_t)0;
        for (int pst = 0; pst < 216; ++pst) {
            seq_t val = dp_cur[pst];
            if (val == (seq_t)0) continue;
            unsigned char cnt = transCnt[pst];
            for (unsigned char t = 0; t < cnt; ++t) {
                int ns = transTo[pst][t];
                dp_next[ns] += val;
            }
        }
        for (int i = 0; i < 216; ++i) dp_cur[i] = dp_next[i];
    }

    /* ---------- Sum over states with no pending cells ---------- */
    seq_t result = (seq_t)0;
    for (int state = 0; state < 216; state += 8) {   /* mask == 0 */
        result += dp_cur[state];
    }
    return result;
}
