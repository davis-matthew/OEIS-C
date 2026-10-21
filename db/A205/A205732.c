/*
 * A205732  Number of (n+1)X5 0..2 arrays with every 2 X 2 subblock having the
 *          same number of clockwise edge increases as its horizontal
 *          neighbors and no 2 X 2 subblock having the same number of
 *          counterclockwise edge increases as its vertical neighbors.
 * https://oeis.org/A205732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of (n+1)×5 0..2 arrays satisfying the OEIS conditions.
 *            Computes a(n) by dynamic programming over states (previous row,
 *            current row). All arithmetic is performed in seq_t
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define ROWS      243                     /* 3^5 possible rows */
#define STATE_COUNT  (ROWS * ROWS)         /* pair of rows */

static bool          cw_eq[STATE_COUNT];
static unsigned char cc_val[STATE_COUNT * 4];   /* cc counts for the 4 blocks */
static size_t        adj_deg[STATE_COUNT];
static size_t*       neighbor_start   = NULL;   /* start index in neighbor_flat */
static size_t*       neighbor_flat    = NULL;   /* flattened adjacency list */
static size_t        total_edges      = 0;

/* ----------------------------------------------------------------------- */
/* Build tables for row decoding, cw/cc values, and adjacency list          */
/* ----------------------------------------------------------------------- */
static void init_tables(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* ----- Decode all possible rows of length 5 in base‑3 ----- */
    static unsigned char row_vals[ROWS][5];
    for (int r = 0; r < ROWS; ++r) {
        int x = r;
        for (int k = 4; k >= 0; --k) {
            row_vals[r][k] = (unsigned char)(x % 3);
            x /= 3;
        }
    }

    /* ----- Pre‑compute cw and cc for each pair (prev, curr) ----- */
    for (int p = 0; p < ROWS; ++p) {
        for (int c = 0; c < ROWS; ++c) {
            size_t id = (size_t)p * ROWS + c;
            int first_cw = -1;
            bool eqcw = true;
            for (int j = 0; j < 4; ++j) {
                int A = row_vals[p][j];
                int B = row_vals[p][j+1];
                int C = row_vals[c][j];
                int D = row_vals[c][j+1];

                int cw = (A < B) + (B < D) + (D < C) + (C < A);
                int cc = (B < A) + (A < C) + (C < D) + (D < B);

                cc_val[(id << 2) + j] = (unsigned char)cc;

                if (j == 0) first_cw = cw;
                else if (cw != first_cw) eqcw = false;
            }
            cw_eq[id] = eqcw;
        }
    }

    /* ----- First pass: count outgoing edges for each state ----- */
    size_t sum = 0;
    for (int p = 0; p < ROWS; ++p) {
        for (int c = 0; c < ROWS; ++c) {
            size_t id = (size_t)p * ROWS + c;
            if (!cw_eq[id]) { adj_deg[id] = 0; continue; }

            size_t deg = 0;
            for (int n = 0; n < ROWS; ++n) {
                size_t id2 = (size_t)c * ROWS + n;
                if (!cw_eq[id2]) continue;

                bool ok = true;
                for (int j = 0; j < 4; ++j) {
                    if (cc_val[(id << 2) + j] == cc_val[(id2 << 2) + j]) {
                        ok = false; break;
                    }
                }
                if (ok) ++deg;
            }
            adj_deg[id] = deg;
            sum += deg;
        }
    }

    /* ----- Allocate adjacency list ----- */
    total_edges = sum;
    neighbor_start = (size_t *)malloc(STATE_COUNT * sizeof(size_t));
    neighbor_flat  = (size_t *)malloc(total_edges * sizeof(size_t));
    if (!neighbor_start || !neighbor_flat) {
        /* Allocation failed; treat as empty graph (will produce zeros). */
        free(neighbor_start); free(neighbor_flat);
        neighbor_start = neighbor_flat = NULL;
        return;
    }

    /* ----- Prefix sums for start indices ----- */
    size_t pos = 0;
    for (size_t id = 0; id < STATE_COUNT; ++id) {
        neighbor_start[id] = pos;
        pos += adj_deg[id];
    }

    /* ----- Second pass: fill adjacency list ----- */
    /* Temporary array to keep current insertion position per state. */
    size_t *cur_pos = (size_t *)calloc(STATE_COUNT, sizeof(size_t));
    if (!cur_pos) {
        free(neighbor_start); free(neighbor_flat);
        neighbor_start = neighbor_flat = NULL;
        return;
    }

    for (int p = 0; p < ROWS; ++p) {
        for (int c = 0; c < ROWS; ++c) {
            size_t id = (size_t)p * ROWS + c;
            if (!cw_eq[id]) continue;

            for (int n = 0; n < ROWS; ++n) {
                size_t id2 = (size_t)c * ROWS + n;
                if (!cw_eq[id2]) continue;

                bool ok = true;
                for (int j = 0; j < 4; ++j) {
                    if (cc_val[(id << 2) + j] == cc_val[(id2 << 2) + j]) {
                        ok = false; break;
                    }
                }
                if (ok) {
                    size_t idx = cur_pos[id]++;
                    neighbor_flat[neighbor_start[id] + idx] = id2;
                }
            }
        }
    }
    free(cur_pos);
}

/* ----------------------------------------------------------------------- */
/* Sequence function                                                        */
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    init_tables();
    if (!neighbor_start || !neighbor_flat) return 0;   /* allocation failed */

    /* Allocate DP arrays */
    seq_t *dp_cur  = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
    seq_t *dp_next = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        free(dp_cur); free(dp_next);
        return 0;
    }

    memset(dp_cur, 0, STATE_COUNT * sizeof(seq_t));

    /* Base: first pair of rows (n = 1) */
    for (size_t id = 0; id < STATE_COUNT; ++id) {
        if (cw_eq[id]) dp_cur[id] = (seq_t)1;
    }

    if (n == 1) {
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < STATE_COUNT; ++i) sum += dp_cur[i];
        free(dp_cur); free(dp_next);
        return sum;
    }

    /* Transition for each additional row */
    for (idx_t step = 2; step <= n; ++step) {
        memset(dp_next, 0, STATE_COUNT * sizeof(seq_t));
        for (size_t s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            size_t start = neighbor_start[s];
            size_t deg   = adj_deg[s];
            for (size_t k = 0; k < deg; ++k) {
                size_t nxt = neighbor_flat[start + k];
                dp_next[nxt] += val;
            }
        }
        /* swap current and next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum up all final states */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < STATE_COUNT; ++i) result += dp_cur[i];

    free(dp_cur); free(dp_next);
    return result;
}
