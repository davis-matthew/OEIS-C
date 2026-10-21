/*
 * A232919  Number of nX7 0..2 arrays with no element x(i,j) adjacent to value
 *          2-x(i,j) horizontally, diagonally or antidiagonally.
 * https://oeis.org/A232919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   pair_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A232919: Number of n×7 0..2 arrays with no element adjacent to 2–x(i,j)
   horizontally or diagonally (upper‑left, upper‑right, lower‑left, lower‑right).
   Vertical adjacency is allowed.  The routine uses a DP over the 192 valid
   column patterns; each transition tests the diagonal restrictions only.
   Complexity O(n) with a constant factor ≈ 200×15. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pair_allowed.h"

#define MAX_COLS 7
#define MAX_ROWS 200      /* fits the 192 admissible column patterns */
#define MAX_OUT 200       /* maximum number of compatible rows for a state */

/* Cached data */
static unsigned char rows[MAX_ROWS][MAX_COLS];
static unsigned short outdeg[MAX_ROWS];
static unsigned char compat_out[MAX_ROWS][MAX_OUT];
static int num_rows = 0;
static bool init_done = false;

/* A 7–column pattern r is compatible with pattern s
   iff for every diagonal neighbour (r[j], s[j±1]) the pair is allowed. */
static inline bool rows_compatible(const unsigned char *r,
                                   const unsigned char *s)
{
    for (int j = 0; j < MAX_COLS; ++j) {
        int a = r[j];
        if (j > 0 && !pair_allowed(a, s[j - 1])) return false;   /* upper‑left / lower‑left */
        if (j < MAX_COLS - 1 && !pair_allowed(a, s[j + 1])) return false; /* upper‑right / lower‑right */
    }
    return true;
}

/* One‑time initialisation of the state table. */
static void init(void)
{
    if (init_done) return;

    int S = 0;
    /* Enumerate all 3^7 = 2187 possible 7‑digit words over {0,1,2}. */
    for (int code = 0; code < 2187; ++code) {
        int tmp = code;
        unsigned char digits[MAX_COLS];
        bool good = true;
        for (int pos = 0; pos < MAX_COLS; ++pos) {
            digits[pos] = tmp % 3;
            tmp /= 3;
        }
        /* Check horizontal adjacency in the same row. */
        for (int j = 0; j < MAX_COLS - 1; ++j) {
            if (!pair_allowed(digits[j], digits[j + 1])) {
                good = false;
                break;
            }
        }
        if (!good) continue;
        for (int k = 0; k < MAX_COLS; ++k) rows[S][k] = digits[k];
        ++S;
    }
    num_rows = S;

    /* Build the compatibility graph (diagonal adjacency only). */
    for (int i = 0; i < num_rows; ++i) {
        outdeg[i] = 0;
        for (int j = 0; j < num_rows; ++j) {
            if (rows_compatible(rows[i], rows[j])) {
                compat_out[i][outdeg[i]++] = j;
            }
        }
    }

    init_done = true;
}

/* Main function required by the specification. */
seq_t a(idx_t n)
{
    init();

    /* dp holds the number of ways to reach each row after the current step. */
    seq_t dp[MAX_ROWS] = {0};
    seq_t next[MAX_ROWS] = {0};

    /* First row: any valid pattern is possible. */
    for (int i = 0; i < num_rows; ++i) dp[i] = 1;

    /* Add more rows. */
    for (idx_t step = 2; step <= n; ++step) {
        for (int j = 0; j < num_rows; ++j) next[j] = 0;
        for (int i = 0; i < num_rows; ++i) {
            seq_t val = dp[i];
            if (val == 0) continue;
            unsigned short deg = outdeg[i];
            for (unsigned short k = 0; k < deg; ++k) {
                int j = compat_out[i][k];
                next[j] += val;
            }
        }
        /* Move to the next step. */
        for (int i = 0; i < num_rows; ++i) dp[i] = next[i];
    }

    /* Sum over all ending rows. */
    seq_t ans = 0;
    for (int i = 0; i < num_rows; ++i) ans += dp[i];
    return ans;
}
