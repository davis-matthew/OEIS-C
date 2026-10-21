/*
 * A207659  Number of nX6 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   build_allowed_table, build_row_table
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207659 – Number of n×6 binary arrays avoiding 0 0 0 and 1 1 1 horizontally
 * and 0 0 1 and 1 0 0 vertically.  Computed by dynamic programming on rows.
 * Row patterns are all 6‑bit strings with no run of three equal bits
 * (there are 26 such patterns).  For n≥3 we keep a table dp[a][b]
 * of the number of arrays of current length whose last two rows are
 * pattern indices a and b.  When extending, we test the vertical
 * restriction for each triple of consecutive rows.  The DP is O(n·26³)
 * and fits easily within the required time limits. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_allowed_table.h"
#include "../utils/build_row_table.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term */
    if (n == 0) return (seq_t)0;

    /* static initialization of tables (once) */
    static bool init = false;
    static uint8_t rows[26];
    static uint8_t pattern_to_idx[64];
    static bool allowed[26][26][26];
    static size_t ROWS;

    if (!init) {
        build_row_table(pattern_to_idx, rows, &ROWS);
        build_allowed_table(rows, ROWS, allowed);
        init = true;
    }

    if (n == 1) return (seq_t)ROWS;                  /* single row */
    if (n == 2) return (seq_t)ROWS * (seq_t)ROWS;    /* all pairs */

    /* DP arrays on the stack: 26*26 = 676 elements */
    seq_t dp[26 * 26];
    seq_t newdp[26 * 26];
    size_t dim = ROWS * ROWS;

    /* initialise for length 2: every pair is possible */
    for (size_t i = 0; i < dim; ++i) dp[i] = (seq_t)1;

    for (idx_t step = 3; step <= n; ++step) {
        /* reset newdp */
        for (size_t i = 0; i < dim; ++i) newdp[i] = (seq_t)0;
        /* transition */
        for (size_t a = 0; a < ROWS; ++a) {
            for (size_t b = 0; b < ROWS; ++b) {
                seq_t val = dp[a * ROWS + b];
                if (!val) continue;
                for (size_t c = 0; c < ROWS; ++c) {
                    if (allowed[a][b][c]) {
                        newdp[b * ROWS + c] += val;
                    }
                }
            }
        }
        /* swap dp and newdp for next step */
        for (size_t i = 0; i < dim; ++i) dp[i] = newdp[i];
    }

    /* sum all dp[b][c] for final count */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < dim; ++i) total += dp[i];

    return total;
}
