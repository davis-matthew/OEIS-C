/*
 * A181210  Number of nX6 binary matrices with no two 1's adjacent diagonally
 *          or antidiagonally.
 * https://oeis.org/A181210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A181210:
 * Number of n×6 binary matrices with no two 1's adjacent diagonally
 * or antidiagonally.  Formula: a(n) counts arrangements of rows such
 * that for any consecutive rows p and q, ((p & (q << 1)) == 0 &&
 * (p & (q >> 1)) == 0).  a(1) = 64, a(2) = 441, …           */

#include <stdbool.h>

static const int COLS   = 6;
static const int STATES = 1 << COLS;

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used, safe guard */

    /* dp_prev[s] is the number of ways to build rows up to the current
       row with the last row in state s. */
    seq_t dp_prev[STATES];
    seq_t dp_next[STATES];

    /* first row: any of the 2^6 patterns is allowed */
    for (int s = 0; s < STATES; ++s) dp_prev[s] = 1;

    /* if only one row, return 2^6 */
    if (n == 1) {
        seq_t total = 0;
        for (int s = 0; s < STATES; ++s) total += dp_prev[s];
        return total;
    }

    /* process rows 2 .. n */
    for (idx_t r = 2; r <= n; ++r) {
        for (int s = 0; s < STATES; ++s) dp_next[s] = 0;

        for (int p = 0; p < STATES; ++p) {
            if (dp_prev[p] == 0) continue;  /* skip zero rows quickly */
            for (int c = 0; c < STATES; ++c) {
                if ((p & ((c << 1) & 0x3F)) == 0 &&
                    (p & ((c >> 1) & 0x3F)) == 0) {
                    dp_next[c] += dp_prev[p];
                }
            }
        }

        /* swap dp_prev and dp_next */
        for (int s = 0; s < STATES; ++s) dp_prev[s] = dp_next[s];
    }

    /* sum all states of the final row */
    seq_t total = 0;
    for (int s = 0; s < STATES; ++s) total += dp_prev[s];
    return total;
}
