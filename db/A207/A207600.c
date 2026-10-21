/*
 * A207600  Number of 3 X n 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207600 – number of 3×n binary arrays avoiding
 *  horizontally 0 0 0 and 0 1 0, and vertically 0 0 1 and 1 0 1.
 *  Computed by a transfer‑matrix DP with 64 states (each state
 *  records the previous two bits of each row).  The DP starts
 *  from the trivial case a(0)=1, enumerates all allowed
 *  pairs of columns for a(1) and a(2), then iteratively
 *  extends the array column by column. */
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Allowed single‑column patterns that satisfy the
     * vertical restrictions (no 001 or 101).  They are
     * coded as 3‑bit numbers with the least significant
     * bit being the top row. */
    const unsigned allowed_cols[6] = {0, 2, 3, 4, 6, 7};

    /* Base cases ---------------------------------------------------- */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;          /* any allowed column */

    /* dp[state] – number of ways for the current length
     * where 'state' encodes the last two columns:
     *   bits 2r     = last column bit at row r
     *   bits 2r + 1 = second last column bit at row r */
    seq_t dp[64]  = {0};
    seq_t newdp[64];

    /* Initialise for length = 2 (no horizontal restrictions apply
     * yet).  For every ordered pair of allowed columns we
     * create a state with prev1 = second column, prev2 = first column. */
    for (int i = 0; i < 6; ++i) {
        unsigned c1 = allowed_cols[i];
        for (int j = 0; j < 6; ++j) {
            unsigned c2 = allowed_cols[j];
            int state = 0;
            for (int r = 0; r < 3; ++r) {
                unsigned b1 = (c2 >> r) & 1u; /* last column */
                unsigned b2 = (c1 >> r) & 1u; /* second last column */
                state |= (b1 << (2 * r));
                state |= (b2 << (2 * r + 1));
            }
            dp[state] += (seq_t)1;
        }
    }

    /* If n == 2 we are done. */
    if (n == 2) {
        seq_t result = 0;
        for (int s = 0; s < 64; ++s) result += dp[s];
        return result;
    }

    /* Extend the array for columns 3 … n */
    for (idx_t col_no = 3; col_no <= n; ++col_no) {
        /* reset newdp */
        for (int s = 0; s < 64; ++s) newdp[s] = (seq_t)0;

        for (int old = 0; old < 64; ++old) {
            seq_t val = dp[old];
            if (val == 0) continue;

            for (int c = 0; c < 6; ++c) {
                unsigned col = allowed_cols[c];
                int new_state = 0;
                bool ok = true;

                for (int r = 0; r < 3; ++r) {
                    unsigned newbit = (col >> r) & 1u;

                    /* Horizontal restriction: newbit==0
                     * cannot appear if the bit two columns back is 0. */
                    unsigned prev2 = (old >> (2 * r + 1)) & 1u;
                    if (prev2 == 0 && newbit == 0) { ok = false; break; }

                    unsigned prev1 = (old >> (2 * r)) & 1u;
                    new_state |= (newbit << (2 * r));
                    new_state |= (prev1 << (2 * r + 1));
                }

                if (!ok) continue;
                newdp[new_state] += val;
            }
        }

        /* copy newdp back to dp for next iteration */
        for (int s = 0; s < 64; ++s) dp[s] = newdp[s];
    }

    /* Sum all states for the final answer */
    seq_t result = 0;
    for (int s = 0; s < 64; ++s) result += dp[s];
    return result;
}
