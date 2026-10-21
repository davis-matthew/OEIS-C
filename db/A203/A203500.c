/*
 * A203500  Number of nX2 0..6 arrays with every nonzero element less than or
 *          equal to some NW, E or S neighbor.
 * https://oeis.org/A203500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of n×2 arrays over {0,…,6} where each non‑zero entry is ≤
   at least one of its North‑West, East, or South neighbors. */
/* The implementation uses a dynamic programming over rows.
   State: previous row values (a,b) and pending south‑requirements
   for that row.  For the new row we enforce the pending
   requirements, check intra‑row constraints, compute the
   pending requirements for the new row, and transition.  
   Each DP state is represented by (row_code*4+mask) where
   row_code = a*7 + b (0 ≤ a,b ≤ 6) and mask is a 2‑bit value.
   The algorithm runs in O(49²·n) time. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Mapping from row code to its column values. */
    static unsigned char row_a[49];
    static unsigned char row_b[49];
    static int initialized = 0;
    if (!initialized) {
        for (unsigned int a = 0; a < 7; ++a)
            for (unsigned int b = 0; b < 7; ++b) {
                unsigned int code = a * 7 + b;
                row_a[code] = (unsigned char)a;
                row_b[code] = (unsigned char)b;
            }
        initialized = 1;
    }

    const int STATES = 49 * 4;   /* 196 states */
    seq_t cur[STATES];
    seq_t next[STATES];

    /* Initialise DP with the first row. */
    for (int i = 0; i < STATES; ++i) cur[i] = (seq_t)0;

    for (int code = 0; code < 49; ++code) {
        unsigned char a1 = row_a[code];
        unsigned char b1 = row_b[code];
        unsigned mask = 0;
        if (a1 > 0 && b1 < a1)       mask |= 1;   /* need South for col 1 */
        if (b1 > 0)                  mask |= 2;   /* col 2 must have South (no NW) */
        cur[code * 4 + mask] += (seq_t)1;
    }

    /* Process remaining rows. */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < STATES; ++i) next[i] = (seq_t)0;

        for (int s = 0; s < STATES; ++s) {
            seq_t ways = cur[s];
            if (ways == (seq_t)0) continue;

            int prev_code = s / 4;
            unsigned mask   = s & 3;
            unsigned char prev_a = row_a[prev_code];
            unsigned char prev_b = row_b[prev_code];

            for (int t = 0; t < 49; ++t) {
                unsigned char a2 = row_a[t];
                unsigned char b2 = row_b[t];

                /* Enforce pending south requirements from previous row. */
                if ((mask & 1) && a2 < prev_a) continue;
                if ((mask & 2) && b2 < prev_b) continue;

                /* Compute new mask for the current row. */
                unsigned new_mask = 0;
                if (a2 > 0 && b2 < a2)                   new_mask |= 1;
                if (b2 > 0 && prev_a < b2)               new_mask |= 2;

                next[t * 4 + new_mask] += ways;
            }
        }

        /* Swap cur and next arrays. */
        for (int i = 0; i < STATES; ++i) cur[i] = next[i];
    }

    /* Sum all states where no pending South requirements remain. */
    seq_t result = (seq_t)0;
    for (int code = 0; code < 49; ++code)
        result += cur[code * 4];

    return result;
}
