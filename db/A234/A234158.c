/*
 * A234158  Number of (n+1)X(5+1) 0..3 arrays with every 2X2 subblock having
 *          the absolute values of all six edge and diagonal differences no
 *          larger than 1.
 * https://oeis.org/A234158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 19,
 *            big any n
 * Helpers:   absdiff_int, val_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A234158 via transfer‑matrix dynamic programming */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/absdiff_int.h"
#include "../utils/val_at.h"

static int initialized = 0;

/* Number of admissible rows (states) for width 6 */
static int states_cnt = 0;
/* Encoded rows as 12‑bit integers (2 bits per entry) */
static uint32_t *states = NULL;
/* Index of first neighbor for each state in neighbor_data */
static int *neighbor_start = NULL;
/* Number of neighbors for each state */
static int *neighbor_deg = NULL;
/* Flattened list of neighbors */
static int *neighbor_data = NULL;

/* DP buffers for current and next states */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* Cache of computed a(n) values (cache[n] holds a(n)) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t last_n = 0;   /* highest n already computed */

/* Build adjacency structure lazily on first call */
static void init(void) {
    /* Step 1: enumerate all 6‑long rows with adjacent value difference ≤1 */
    uint64_t id_map[4096];
    for (int i = 0; i < 4096; ++i) id_map[i] = (uint64_t)-1;
    uint32_t tmp_states[4096];
    for (uint32_t mask = 0; mask < 4096; ++mask) {
        int ok = 1;
        for (int c = 0; c < 5 && ok; ++c) {
            int v = val_at(mask, c);
            int w = val_at(mask, c + 1);
            if (absdiff_int(v, w) > 1) ok = 0;
        }
        if (ok) {
            id_map[mask] = states_cnt;
            tmp_states[states_cnt] = mask;
            ++states_cnt;
        }
    }
    states = (uint32_t*)malloc(states_cnt * sizeof(uint32_t));
    memcpy(states, tmp_states, states_cnt * sizeof(uint32_t));

    /* Step 2: first pass – degree counts */
    neighbor_deg = (int*)calloc(states_cnt, sizeof(int));
    int total_edges = 0;
    for (int i = 0; i < states_cnt; ++i) {
        uint32_t row_i = states[i];
        for (int j = 0; j < states_cnt; ++j) {
            uint32_t row_j = states[j];
            int ok = 1;
            for (int c = 0; c < 5 && ok; ++c) {
                int xi = val_at(row_i, c);
                int xi1 = val_at(row_i, c + 1);
                int yj = val_at(row_j, c);
                int yj1 = val_at(row_j, c + 1);
                if (absdiff_int(xi, yj) > 1) ok = 0;
                if (absdiff_int(xi1, yj1) > 1) ok = 0;
                if (absdiff_int(xi, yj1) > 1) ok = 0;
                if (absdiff_int(xi1, yj) > 1) ok = 0;
            }
            if (ok) {
                ++neighbor_deg[i];
                ++total_edges;
            }
        }
    }

    /* Step 3: prefix sums for start indices */
    neighbor_start = (int*)calloc(states_cnt, sizeof(int));
    int acc = 0;
    for (int i = 0; i < states_cnt; ++i) {
        neighbor_start[i] = acc;
        acc += neighbor_deg[i];
    }

    /* Step 4: second pass – fill neighbor list */
    neighbor_data = (int*)malloc(total_edges * sizeof(int));
    int *tmp_pos = (int*)malloc(states_cnt * sizeof(int));
    memcpy(tmp_pos, neighbor_start, states_cnt * sizeof(int));
    for (int i = 0; i < states_cnt; ++i) {
        uint32_t row_i = states[i];
        for (int j = 0; j < states_cnt; ++j) {
            uint32_t row_j = states[j];
            int ok = 1;
            for (int c = 0; c < 5 && ok; ++c) {
                int xi = val_at(row_i, c);
                int xi1 = val_at(row_i, c + 1);
                int yj = val_at(row_j, c);
                int yj1 = val_at(row_j, c + 1);
                if (absdiff_int(xi, yj) > 1) ok = 0;
                if (absdiff_int(xi1, yj1) > 1) ok = 0;
                if (absdiff_int(xi, yj1) > 1) ok = 0;
                if (absdiff_int(xi1, yj) > 1) ok = 0;
            }
            if (ok) {
                int pos = tmp_pos[i]++;
                neighbor_data[pos] = j;
            }
        }
    }
    free(tmp_pos);

    /* Step 5: DP buffers */
    dp_cur = (seq_t*)calloc(states_cnt, sizeof(seq_t));
    dp_next = (seq_t*)calloc(states_cnt, sizeof(seq_t));
    for (int i = 0; i < states_cnt; ++i) dp_cur[i] = (seq_t)1;

    /* Step 6: cache */
    cache_cap = 16;
    cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
    cache[0] = (seq_t)0;   /* unused, placeholder */
    last_n = 0;

    initialized = 1;
}

seq_t a(idx_t n) {
    if (!initialized) init();

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute further terms up to n */
    while (last_n < (size_t)n) {
        /* reset dp_next */
        memset(dp_next, 0, states_cnt * sizeof(seq_t));
        for (int i = 0; i < states_cnt; ++i) {
            seq_t val = dp_cur[i];
            if (val == (seq_t)0) continue;
            int start = neighbor_start[i];
            int deg = neighbor_deg[i];
            int *neigh = neighbor_data + start;
            for (int k = 0; k < deg; ++k) {
                dp_next[neigh[k]] += val;
            }
        }
        /* sum results to get a(last_n+1) */
        seq_t sum = 0;
        for (int i = 0; i < states_cnt; ++i) sum += dp_next[i];
        cache[++last_n] = sum;
        /* swap dp_cur and dp_next for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    return cache[n];
}
