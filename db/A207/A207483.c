/*
 * A207483  Number of n X 3 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207483: number of n×3 binary arrays avoiding
   horizontal pattern 000 and 011, and vertical patterns 011 and 110.
   Computed by a simple dynamic programming over rows; the state
   consists of the last two rows (each of length 3 bits).  The
   allowed rows are {001,010,100,101,110,111}.  The function
   works for any n≥1 and returns the exact count in type seq_t. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases for n = 1 and n = 2 */
    if (n == 0) return 0;          /* safety for n < 1 */
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)36;

    /* Allowed row patterns (3‑bit numbers) */
    const int allowed[6] = {1, 2, 4, 5, 6, 7};

    /* DP arrays: curr and next state counts (size 36 = 6×6 pairs) */
    seq_t curr[36] = {0};
    seq_t next[36] = {0};

    /* Initialisation: after two rows, each allowed pair occurs once */
    for (idx_t i = 0; i < 6; ++i)
        for (idx_t j = 0; j < 6; ++j)
            curr[i * 6 + j] = (seq_t)1;

    /* Main DP loop for rows 3 … n */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear next array */
        for (idx_t k = 0; k < 36; ++k) next[k] = 0;

        /* iterate over all state pairs (prev2, prev1) */
        for (idx_t s = 0; s < 36; ++s) {
            seq_t val = curr[s];
            if (val == 0) continue;                 /* no sequences for this state */
            int prev2 = allowed[s / 6];              /* bits of the row two steps back */
            int prev1 = allowed[s % 6];              /* bits of the previous row   */

            /* try all possible new rows c */
            for (idx_t c_idx = 0; c_idx < 6; ++c_idx) {
                int cur = allowed[c_idx];

                /* check every column for vertical forbidden patterns */
                int ok = 1;
                for (int col = 0; col < 3; ++col) {
                    int mask = 1 << (2 - col);        /* bit position for this column */
                    int x = (prev2 & mask) ? 1 : 0;
                    int y = (prev1 & mask) ? 1 : 0;
                    int z = (cur   & mask) ? 1 : 0;
                    int pat = (x << 2) | (y << 1) | z;
                    if (pat == 3 || pat == 6) {       /* 011 or 110 forbidden */
                        ok = 0;
                        break;
                    }
                }
                if (!ok) continue;

                /* new state: (prev1, cur) */
                idx_t new_state = (s % 6) * 6 + c_idx;
                next[new_state] += val;
            }
        }
        /* move to next row */
        for (idx_t k = 0; k < 36; ++k) curr[k] = next[k];
    }

    /* Sum over all ending states */
    seq_t total = 0;
    for (idx_t k = 0; k < 36; ++k) total += curr[k];
    return total;
}
