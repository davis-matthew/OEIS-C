/*
 * A206252  Number of (n+1)X6 0..3 arrays with every 2X2 subblock having zero
 *          permanent.
 * https://oeis.org/A206252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of (n+1)×6 matrices with entries in
 *            {0,1,2,3} such that every 2×2 subblock has zero permanent. It is
 *            computed by a transfer‑matrix method: each row of the matrix is
 *            a pattern of length 6 over {0,1,2,3}. Two successive rows are
 *            compatible iff for every adjacent pair of colum...
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define S_ROWS       4096          /* 4⁶ possible row patterns */
#define MAX_DIGITS   6

/* Static data used across calls – built only once. */
static bool            adjacency_built = false;
static uint16_t        *adjacent = NULL;       /* concatenated adjacency lists */
static int             *adj_start = NULL;      /* start index of each row’s list */
static uint8_t        (*row_digits)[MAX_DIGITS] = NULL;

/* Build the adjacency structure once. */
static void build_adjacency(void)
{
    if (adjacency_built) return;

    /* Allocate and compute the digit tables for all row patterns. */
    row_digits = (uint8_t (*)[MAX_DIGITS])malloc(S_ROWS * sizeof(*row_digits));
    for (int p = 0; p < S_ROWS; ++p) {
        for (int c = 0; c < MAX_DIGITS; ++c)
            row_digits[p][c] = (p >> (c * 2)) & 3U;
    }

    /* First pass: count outgoing edges for each pattern. */
    uint16_t *degree = (uint16_t *)malloc(S_ROWS * sizeof(uint16_t));
    int total_edges = 0;
    for (int p = 0; p < S_ROWS; ++p) {
        uint16_t d = 0;
        uint8_t *dp = row_digits[p];
        for (int q = 0; q < S_ROWS; ++q) {
            uint8_t *dq = row_digits[q];
            bool ok = true;
            for (int j = 0; j < 5; ++j) {
                int a = dp[j];
                int b = dp[j + 1];
                int c = dq[j];
                int d2 = dq[j + 1];
                if (a * d2 + b * c != 0) { ok = false; break; }
            }
            if (ok) ++d;
        }
        degree[p] = d;
        total_edges += d;
    }

    /* Allocate storage for adjacency lists. */
    adjacent = (uint16_t *)malloc(total_edges * sizeof(uint16_t));
    adj_start = (int *)malloc((S_ROWS + 1) * sizeof(int));

    /* Second pass: fill adjacency lists. */
    int pos = 0;
    for (int p = 0; p < S_ROWS; ++p) {
        adj_start[p] = pos;
        uint8_t *dp = row_digits[p];
        for (int q = 0; q < S_ROWS; ++q) {
            uint8_t *dq = row_digits[q];
            bool ok = true;
            for (int j = 0; j < 5; ++j) {
                int a = dp[j];
                int b = dp[j + 1];
                int c = dq[j];
                int d2 = dq[j + 1];
                if (a * d2 + b * c != 0) { ok = false; break; }
            }
            if (ok) adjacent[pos++] = (uint16_t)q;
        }
    }
    adj_start[S_ROWS] = pos;   /* sentinel */

    /* Clean up temporary data. */
    free(degree);
    free(row_digits);
    adjacency_built = true;
}

/* Dynamic programming arrays (shared across calls). */
static bool dp_alloced = false;
static seq_t *dp_prev = NULL;
static seq_t *dp_curr = NULL;

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure the adjacency structure is ready. */
    build_adjacency();

    /* Allocate DP arrays once. */
    if (!dp_alloced) {
        dp_prev = (seq_t *)malloc(S_ROWS * sizeof(seq_t));
        dp_curr = (seq_t *)malloc(S_ROWS * sizeof(seq_t));
        dp_alloced = true;
    }

    /* Initialize DP for the first row – any pattern is allowed. */
    for (int i = 0; i < S_ROWS; ++i) dp_prev[i] = (seq_t)1;

    /* Perform n transitions (n+1 rows total). */
    for (idx_t step = 1; step <= n; ++step) {
        /* Zero the current DP array. */
        for (int i = 0; i < S_ROWS; ++i) dp_curr[i] = (seq_t)0;

        /* Transition from dp_prev to dp_curr. */
        for (int p = 0; p < S_ROWS; ++p) {
            seq_t val = dp_prev[p];
            if (val == (seq_t)0) continue;
            int start = adj_start[p];
            int end   = adj_start[p + 1];
            for (int idx = start; idx < end; ++idx) {
                uint16_t q = adjacent[idx];
                dp_curr[q] += val;
            }
        }
        /* Swap DP arrays. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    /* Sum over all possible last rows. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < S_ROWS; ++i) result += dp_prev[i];

    return result;
}
