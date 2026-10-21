/*
 * A208707  Number of n X 6 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays with: (1) The first cell (row
 *            1, column 1) is 0. (2) Scanning row‑major order, no element
 *            equals more than two of its immediate leftward, upward or
 *            right‑upward antidiagonal neighbors. This is equivalent to the
 *            DP described below. (The recurrence given in OEIS,...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* 64 masks for a row of 6 columns. */
#define MASK_COUNT 64

/* Precomputed legality of transitions between rows. */
static bool good[MASK_COUNT][MASK_COUNT];
static bool good_initialized = false;

/* Computes whether a transition from a previous row (prev) to a new row
   (curr) satisfies the neighbor condition for all cells. */
static void init_good(void)
{
    if (good_initialized) return;
    for (int prev = 0; prev < MASK_COUNT; ++prev) {
        for (int curr = 0; curr < MASK_COUNT; ++curr) {
            bool ok = true;
            for (int col = 0; col < 6; ++col) {
                unsigned int curr_val = (curr >> col) & 1U;
                /* Count equal neighbors among: left (same row), up, diag-up-right. */
                unsigned int eq = 0;
                if (col > 0) {
                    unsigned int left = (curr >> (col - 1)) & 1U;
                    if (left == curr_val) ++eq;
                }
                unsigned int up = (prev >> col) & 1U;
                if (up == curr_val) ++eq;
                if (col + 1 < 6) {
                    unsigned int diag = (prev >> (col + 1)) & 1U;
                    if (diag == curr_val) ++eq;
                }
                if (eq > 2) { ok = false; break; }
            }
            good[prev][curr] = ok;
        }
    }
    good_initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* Undefined for offset 1; return 0. */

    /* Ensure transition table is ready. */
    init_good();

    /* DP arrays for the previous and current row. */
    seq_t dp_prev[MASK_COUNT];
    seq_t dp_cur[MASK_COUNT];

    /* Row 1: only masks with bit 0 equal to 0 (first cell must be 0). */
    for (int m = 0; m < MASK_COUNT; ++m)
        dp_prev[m] = ((m & 1) == 0) ? (seq_t)1 : (seq_t)0;

    if (n == 1) {
        seq_t sum = 0;
        for (int m = 0; m < MASK_COUNT; ++m) sum += dp_prev[m];
        return sum;
    }

    /* Iterate over rows 2 .. n. */
    for (idx_t r = 2; r <= n; ++r) {
        for (int m = 0; m < MASK_COUNT; ++m) dp_cur[m] = 0;
        for (int prev = 0; prev < MASK_COUNT; ++prev) {
            seq_t val = dp_prev[prev];
            if (val == 0) continue;
            for (int curr = 0; curr < MASK_COUNT; ++curr) {
                if (good[prev][curr]) dp_cur[curr] += val;
            }
        }
        /* Move to next row. */
        for (int m = 0; m < MASK_COUNT; ++m) dp_prev[m] = dp_cur[m];
    }

    /* Sum over all row patterns for row n. */
    seq_t result = 0;
    for (int m = 0; m < MASK_COUNT; ++m) result += dp_prev[m];
    return result;
}
