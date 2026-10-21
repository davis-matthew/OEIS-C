/*
 * A234547  Number of (n+1) X (5+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×6 arrays of values 0..5 such that in
 *            every 2×2 subblock the difference between its diagonal sum and
 *            its anti‑diagonal sum equals ±3. The implementation uses a
 *            transfer‑matrix (transfer‑graph) approach: each row is a state
 *            of 6 base‑6 digits; a directed edge indicates two co...
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   build_next_row, digits_to_state
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_next_row.h"
#include "../utils/digits_to_state.h"

#define STATES 46656          // 6^6 possible rows
#define MAX_EDGES_PER_STATE 200   // ≤192

/* Pre‑computed table of row digits */
static uint8_t row_digits[STATES][6];
/* Adjacency lists: adj[s][k] is the k‑th next state from state s */
static uint16_t adj[STATES][MAX_EDGES_PER_STATE];
static uint8_t adj_len[STATES];

/* Flag indicating that the static tables have been initialised */
static bool tables_initialised = false;

/* One‑time construction of state digits table and adjacency lists */
static void init_tables(void) {
    if (tables_initialised) return;
    tables_initialised = true;
    /* Compute digit representation for each state id */
    for (int id = 0; id < STATES; ++id) {
        int t = id;
        for (int i = 5; i >= 0; --i) {
            row_digits[id][i] = t % 6;
            t /= 6;
        }
    }
    /* Build adjacency lists */
    for (int s = 0; s < STATES; ++s) {
        const uint8_t *prev = row_digits[s];
        uint8_t next[6];
        int cnt = 0;
        for (int n0 = 0; n0 < 6; ++n0) {
            next[0] = (uint8_t)n0;
            build_next_row(prev, 0, next, adj[s], &cnt);
        }
        adj_len[s] = (uint8_t)cnt;   // maximum 192
    }
}

/* DP buffers (allocated on first call) */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* Main function required by the framework */
seq_t a(idx_t n) {
    /* Allocate DP buffers once */
    if (!dp_cur) {
        dp_cur = (seq_t *)malloc(STATES * sizeof(seq_t));
        dp_next = (seq_t *)malloc(STATES * sizeof(seq_t));
    }

    init_tables();

    /* Number of rows in the grid is n+1 */
    idx_t L = n + 1;

    /* dp_cur[s] = number of ways to end with state s after current step */
    for (int s = 0; s < STATES; ++s)
        dp_cur[s] = 1;        // first row may be any state

    /* Perform n transitions (L-1) */
    for (idx_t step = 1; step < L; ++step) {
        for (int s = 0; s < STATES; ++s)
            dp_next[s] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            uint8_t len = adj_len[s];
            uint16_t *list = adj[s];
            for (int k = 0; k < len; ++k) {
                int t = list[k];
                dp_next[t] += val;
            }
        }

        /* Swap current and next DP arrays */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum over all possible last rows */
    seq_t result = 0;
    for (int s = 0; s < STATES; ++s)
        result += dp_cur[s];

    return result;
}
