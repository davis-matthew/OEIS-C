/*
 * A183379  Number of nX6 binary arrays with no element equal to the mod 3 sum
 *          of its king-move neighbors.
 * https://oeis.org/A183379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays with no element equal to the
 *            mod 3 sum of its king‑move neighbors. The implementation uses a
 *            transfer‑matrix DP over pairs of consecutive rows. States are
 *            pairs (prev_row, curr_row) where prev_row may be a real row
 *            (0…63) or a special “none” symbol (index 64). F...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Number of possible row patterns for width 6 */
#define ROWS 64
/* Use 64 to represent a “missing” row (no neighbors above/below) */
#define NONE 64
/* Number of state rows including NONE */
#define STATES (ROWS + 1)

/* Precomputed masks of valid next rows for each state pair */
static uint64_t next_masks[STATES][ROWS] = {0};
/* Precomputed validity of last row given its above row */
static bool last_valid[STATES][ROWS] = {0};
/* Flag whether tables have been initialized */
static bool tables_built = false;

/* Checks whether a single row is valid given its above and below rows.
   If above/ below is NONE (-1) they are treated as absent. */
static bool row_valid(int above, int cur, int below)
{
    for (int j = 0; j < 6; ++j) {
        int sum = 0;
        /* Top row */
        if (above >= 0) {
            if (j > 0) sum += (above >> (j - 1)) & 1;
            sum += (above >> j) & 1;
            if (j < 5) sum += (above >> (j + 1)) & 1;
        }
        /* Same row left/right */
        if (j > 0) sum += (cur >> (j - 1)) & 1;
        if (j < 5) sum += (cur >> (j + 1)) & 1;
        /* Bottom row */
        if (below >= 0) {
            if (j > 0) sum += (below >> (j - 1)) & 1;
            sum += (below >> j) & 1;
            if (j < 5) sum += (below >> (j + 1)) & 1;
        }
        int val = (cur >> j) & 1;
        if (val == (sum % 3))
            return false;  /* invalid */
    }
    return true;          /* all cells satisfied */
}

/* Build the lookup tables once */
static void build_tables(void)
{
    if (tables_built) return;
    for (int p = 0; p < STATES; ++p) {
        int above = (p == NONE) ? -1 : p;
        for (int c = 0; c < ROWS; ++c) {
            uint64_t mask = 0;
            for (int n = 0; n < ROWS; ++n) {
                if (row_valid(above, c, n))
                    mask |= (uint64_t)1 << n;
            }
            next_masks[p][c] = mask;
            /* Precompute validity of last row after all rows are added */
            last_valid[p][c] = row_valid(above, c, -1);
        }
    }
    tables_built = true;
}

/* Main function required by the build system */
seq_t a(idx_t n)
{
    build_tables();   /* ensure tables are ready */

    if (n == 0) return 0;  /* undefined offset, but no harm */

    /* Handle the trivial case n = 1 directly */
    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < ROWS; ++r) {
            if (row_valid(-1, r, -1))
                ans += (seq_t)1;
        }
        return ans;
    }

    /* DP arrays: dp[prev_row][curr_row] */
    seq_t dp[STATES][ROWS] = {0};
    seq_t ndp[STATES][ROWS] = {0};

    /* Initial state: only first row, with no above row */
    for (int r = 0; r < ROWS; ++r)
        dp[NONE][r] = (seq_t)1;

    /* Iterate over each additional row */
    for (idx_t step = 1; step < n; ++step) {
        /* Zero ndp */
        for (int p = 0; p < STATES; ++p)
            for (int c = 0; c < ROWS; ++c)
                ndp[p][c] = (seq_t)0;

        for (int p = 0; p < STATES; ++p) {
            for (int c = 0; c < ROWS; ++c) {
                seq_t ways = dp[p][c];
                if (ways == (seq_t)0) continue;
                uint64_t mask = next_masks[p][c];
                while (mask) {
                    uint64_t bit = mask & -mask;
                    int nrow = __builtin_ctzll(mask);
                    ndp[c][nrow] += ways;
                    mask ^= bit;
                }
            }
        }
        /* Copy ndp to dp for next iteration */
        for (int p = 0; p < STATES; ++p)
            for (int c = 0; c < ROWS; ++c)
                dp[p][c] = ndp[p][c];
    }

    /* Sum over states where the last row is valid (no below row) */
    seq_t ans = 0;
    for (int p = 0; p < STATES; ++p) {
        for (int c = 0; c < ROWS; ++c) {
            if (dp[p][c] == (seq_t)0) continue;
            if (last_valid[p][c])
                ans += dp[p][c];
        }
    }
    return ans;
}
