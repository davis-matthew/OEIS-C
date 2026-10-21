/*
 * A225022  Number of (n+1)X4 0..2 matrices with each 2X2 permanent equal.
 * https://oeis.org/A225022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A225022: number of (n+1)×4 matrices with entries 0..2 whose every
    2×2 permanent is equal.
    The implementation uses dynamic programming over consecutive rows.
    A single row is a 4‑tuple of values in {0,1,2} (81 possibilities).
    For two rows rprev and rnext we pre‑compute the common permanent value
    (0..8) if all three 2×2 permanents are equal, otherwise -1.
    A DP state is (last_row, common_permanent).  After processing two rows
    the DP array curr holds the number of sequences that end with each
    state.  For each further row we transition only to states with the
    same permanent value.
    The algorithm runs in O(n · 81²) time and uses a few thousand
    seq_t elements, easily meeting the 2 s requirement even for n≈10000. */

#include <stdbool.h>
#include <stdint.h>

enum { NUM_ROWS = 81, NUM_P = 9 };

/* ------------------------------------------------------------------- */
/*  Main routine                                                     */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* transition_p[i][j] = common permanent (0..8) if valid,
       or -1 if the 2×2 permanents differ.                               */
    static int8_t transition_p[NUM_ROWS][NUM_ROWS];
    static bool initialized = false;

    if (!initialized) {
        /* Encode each row as a 4‑digit base‑3 number (rows[0]..rows[3]). */
        uint8_t rows[NUM_ROWS][4];
        for (int idx = 0; idx < NUM_ROWS; ++idx) {
            int t = idx;
            for (int c = 0; c < 4; ++c) {
                rows[idx][c] = t % 3;
                t /= 3;
            }
        }

        /* Fill the transition table. */
        for (int i = 0; i < NUM_ROWS; ++i) {
            for (int j = 0; j < NUM_ROWS; ++j) {
                int p0 = rows[i][0] * rows[j][1] + rows[i][1] * rows[j][0];
                int p1 = rows[i][1] * rows[j][2] + rows[i][2] * rows[j][1];
                int p2 = rows[i][2] * rows[j][3] + rows[i][3] * rows[j][2];
                transition_p[i][j] = (p0 == p1 && p1 == p2) ? (int8_t)p0 : -1;
            }
        }
        initialized = true;
    }

    /* DP arrays: for each last row and common permanent value. */
    seq_t curr[NUM_ROWS][NUM_P];
    seq_t next[NUM_ROWS][NUM_P];

    /* Initialise with a single first row.  The initial permanent is
       undefined, so we start after the first transition (two rows).      */
    for (int i = 0; i < NUM_ROWS; ++i)
        for (int p = 0; p < NUM_P; ++p)
            curr[i][p] = 0;

    /* First transition: from first row (any) to second row.             */
    for (int rprev = 0; rprev < NUM_ROWS; ++rprev) {
        for (int rnext = 0; rnext < NUM_ROWS; ++rnext) {
            int8_t p = transition_p[rprev][rnext];
            if (p >= 0)
                curr[rnext][p] += 1;   /* each first row occurs once */
        }
    }

    /* Remaining transitions (n-2 more, i.e. total of n-1 iterations). */
    for (idx_t trans = 1; trans < n; ++trans) {
        /* Zero next array. */
        for (int i = 0; i < NUM_ROWS; ++i)
            for (int p = 0; p < NUM_P; ++p)
                next[i][p] = 0;

        /* Transition from current states to next row. */
        for (int rprev = 0; rprev < NUM_ROWS; ++rprev) {
            for (int pprev = 0; pprev < NUM_P; ++pprev) {
                seq_t cnt = curr[rprev][pprev];
                if (cnt == 0) continue;
                for (int rnext = 0; rnext < NUM_ROWS; ++rnext) {
                    if (transition_p[rprev][rnext] == pprev)
                        next[rnext][pprev] += cnt;
                }
            }
        }

        /* Move next to curr for the next step. */
        for (int i = 0; i < NUM_ROWS; ++i)
            for (int p = 0; p < NUM_P; ++p)
                curr[i][p] = next[i][p];
    }

    /* Sum all terminal states to obtain a(n). */
    seq_t total = 0;
    for (int i = 0; i < NUM_ROWS; ++i)
        for (int p = 0; p < NUM_P; ++p)
            total += curr[i][p];

    return total;
}
