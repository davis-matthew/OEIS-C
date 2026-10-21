/*
 * A233814  Number of (n+1)X(4+1) 0..5 arrays with every 2X2 subblock having
 *          the sum of the squares of all six edge and diagonal differences
 *          equal to 11.
 * https://oeis.org/A233814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays with entries in {0,…,5} such
 *            that every 2×2 subblock has the sum of the squares of its six
 *            edge and diagonal differences equal to 11. The result is
 *            obtained by dynamic programming over all 6⁵ possible rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   row_pair_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/row_pair_ok.h"

#define N_STATES 7776  /* 6⁵ */

static bool built = false;
static idx_t *offsets = NULL;      /* size N_STATES+1 */
static idx_t *edges   = NULL;      /* flattened adjacency lists */
static seq_t *dp_cur  = NULL;      /* DP vector for current row */
static seq_t *dp_next = NULL;      /* DP vector for next row */

/* Decode one row into 5 bytes (values 0..5). */
static void decode_row(idx_t r, uint8_t row[5]) {
    idx_t t = r;
    for (idx_t k = 0; k < 5; ++k) {
        row[k] = (uint8_t)(t % 6);
        t /= 6;
    }
}

/* Initialize the state space and DP tables. */
static void init(void) {
    /* Allocate rows buffer and decode all states. */
    uint8_t (*rows)[5] = (uint8_t (*)[5]) malloc(N_STATES * sizeof(uint8_t[5]));
    for (idx_t r = 0; r < N_STATES; ++r)
        decode_row(r, rows[r]);

    /* First pass: count degrees. */
    idx_t *deg = (idx_t *) calloc(N_STATES, sizeof(idx_t));
    uint8_t row_i[5], row_j[5];
    for (idx_t i = 0; i < N_STATES; ++i) {
        for (idx_t j = i; j < N_STATES; ++j) {
            if (row_pair_ok(rows[i], rows[j])) {
                ++deg[i];
                if (i != j) ++deg[j];
            }
        }
    }

    /* Build offsets array. */
    offsets = (idx_t *) malloc((N_STATES + 1) * sizeof(idx_t));
    idx_t total_edges = 0;
    for (idx_t i = 0; i < N_STATES; ++i) {
        offsets[i] = total_edges;
        total_edges += deg[i];
    }
    offsets[N_STATES] = total_edges;

    /* Allocate edges array. */
    edges = (idx_t *) malloc(total_edges * sizeof(idx_t));

    /* Second pass: fill adjacency lists. */
    idx_t *pos = (idx_t *) malloc(N_STATES * sizeof(idx_t));
    for (idx_t i = 0; i < N_STATES; ++i) pos[i] = offsets[i];

    for (idx_t i = 0; i < N_STATES; ++i) {
        for (idx_t j = i; j < N_STATES; ++j) {
            if (row_pair_ok(rows[i], rows[j])) {
                edges[pos[i]++] = j;
                if (i != j)
                    edges[pos[j]++] = i;
            }
        }
    }

    free(pos);
    free(deg);
    free(rows);

    /* Allocate DP vectors. */
    dp_cur  = (seq_t *) calloc(N_STATES, sizeof(seq_t));
    dp_next = (seq_t *) calloc(N_STATES, sizeof(seq_t));

    built = true;
}

seq_t a(idx_t n) {
    if (!built) init();

    if (n == 0) return (seq_t)0;

    /* Initial state: any first row is allowed once. */
    for (idx_t i = 0; i < N_STATES; ++i)
        dp_cur[i] = (seq_t)1;

    /* Perform n transitions (rows 1 → n). */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero next DP vector. */
        for (idx_t i = 0; i < N_STATES; ++i)
            dp_next[i] = (seq_t)0;

        for (idx_t i = 0; i < N_STATES; ++i) {
            seq_t cnt = dp_cur[i];
            if (cnt == (seq_t)0) continue;
            for (idx_t e = offsets[i]; e < offsets[i + 1]; ++e) {
                idx_t j = edges[e];
                dp_next[j] += cnt;
            }
        }
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all ending states. */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < N_STATES; ++i)
        result += dp_cur[i];

    return result;
}
