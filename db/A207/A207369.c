/*
 * A207369  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A207369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function computes OEIS A207369 – the number of 4×n binary
 *            matrices avoiding * the horizontal patterns 000 and 011 and the
 *            vertical patterns 001 and 011. * It uses a dynamic‑programming
 *            approach over the 9 allowable columns of a 4‑row * column (those
 *            that satisfy the vertical constraints). Th...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 24, u64 n <= 60, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* List of the 9 column bit patterns that satisfy the vertical constraints
 * (no 0 0 1 or 0 1 1 vertically).  The patterns are numbered 0..8 for
 * convenience and stored as 4‑bit values (from row 0 to row 3, top to bottom). */
static const int allowed[9] = {0, 4, 5, 8, 10, 12, 13, 14, 15};

/* valid[a][b][c] is true if placing column c after columns a,b does
 * not create a forbidden horizontal pattern in any row.  The table is
 * 9×9×9 = 729 booleans. */
static bool valid[9][9][9];
static bool valid_initialized = false;

/* Pre‑compute the valid table once. */
static void init_valid(void)
{
    int bits[9][4];
    for (int i = 0; i < 9; ++i) {
        int v = allowed[i];
        for (int r = 0; r < 4; ++r)
            bits[i][r] = (v >> (3 - r)) & 1;
    }
    for (int a = 0; a < 9; ++a)
        for (int b = 0; b < 9; ++b)
            for (int c = 0; c < 9; ++c) {
                bool ok = true;
                for (int r = 0; r < 4; ++r) {
                    int a0 = bits[a][r], b0 = bits[b][r], c0 = bits[c][r];
                    if ((a0 == 0 && b0 == 0 && c0 == 0) ||
                        (a0 == 0 && b0 == 1 && c0 == 1)) {
                        ok = false;
                        break;
                    }
                }
                valid[a][b][c] = ok;
            }
    valid_initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle trivial cases directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)81;

    /* Ensure the valid table is ready (once, even for repeated calls) */
    if (!valid_initialized) init_valid();

    /* DP arrays: 81 = 9 × 9 states (previous two columns) */
    static seq_t dp_prev[81];
    static seq_t dp_next[81];
    /* Initialise dp_prev for columns 1 and 2: any pair of allowed columns
     * is valid because horizontal constraints need at least 3 columns. */
    for (int i = 0; i < 81; ++i) dp_prev[i] = (seq_t)1;

    /* Iterate from length 3 up to n */
    for (idx_t len = 3; len <= n; ++len) {
        /* Clear dp_next */
        for (int i = 0; i < 81; ++i) dp_next[i] = (seq_t)0;

        /* Transition */
        for (int s = 0; s < 81; ++s) {
            seq_t cnt = dp_prev[s];
            if (cnt == (seq_t)0) continue;
            int prev_prev = s / 9;
            int prev = s % 9;
            for (int nxt = 0; nxt < 9; ++nxt) {
                if (!valid[prev_prev][prev][nxt]) continue;
                int t = prev * 9 + nxt;
                dp_next[t] += cnt;
            }
        }

        /* After transition, if this is the target length, compute total */
        if (len == n) {
            seq_t total = (seq_t)0;
            for (int i = 0; i < 81; ++i) total += dp_next[i];
            return total;
        }

        /* Prepare for next iteration: dp_prev becomes dp_next */
        for (int i = 0; i < 81; ++i) dp_prev[i] = dp_next[i];
    }

    /* Should never reach here for n >= 3, but keep the compiler happy */
    return (seq_t)0;
}
