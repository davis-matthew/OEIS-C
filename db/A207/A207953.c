/*
 * A207953  Number of 7Xn 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 14, u64 n <= 111, u128 n <= 210,
 *            big any n
 * Helpers:   has_vertical_forbidden, is_allowed_transition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/has_vertical_forbidden.h"
#include "../utils/is_allowed_transition.h"

/* A207953: Number of 7×n 0..1 arrays avoiding horizontal 001,011
 * and vertical 010,101.  Computed by dynamic programming over
 * 7‑bit column masks. */

/* Global state for DP */
static bool initialized = false;
static idx_t V = 0;                 /* number of valid columns            */
static uint8_t *valid_masks = NULL; /* list of valid column masks         */
static uint8_t *mask_to_idx = NULL; /* mapping mask -> index or 0xFF     */
static uint8_t *trans_flat = NULL;  /* flattened list of transitions     */
static uint8_t *trans_count = NULL;/* number of transitions per pair     */
static seq_t *cur = NULL;           /* DP for current length             */
static seq_t *nxt = NULL;           /* DP for next length                */
static idx_t cur_len = 0;           /* current number of columns processed*/
static size_t state_count = 0;      /* V × V                              */
static seq_t *cache = NULL;         /* memoized a(n) values              */
static idx_t cache_size = 0;        /* allocated size of cache          */

/* Initialize all data structures */
static void init_sequence(void) {
    /* 1) Find all valid column masks */
    uint8_t temp[128];
    uint8_t cnt = 0;
    for (uint16_t m = 0; m < 128; ++m) {
        if (!has_vertical_forbidden((uint8_t)m)) {
            temp[cnt++] = (uint8_t)m;
        }
    }
    V = cnt;
    valid_masks = (uint8_t*)malloc(V * sizeof(uint8_t));
    mask_to_idx = (uint8_t*)malloc(128 * sizeof(uint8_t));
    for (idx_t i = 0; i < V; ++i) valid_masks[i] = temp[i];
    for (int i = 0; i < 128; ++i) mask_to_idx[i] = 0xFFu;
    for (idx_t i = 0; i < V; ++i) mask_to_idx[valid_masks[i]] = (uint8_t)i;

    /* 2) Pre‑compute transitions for every ordered pair (p,q) */
    state_count = (size_t)V * (size_t)V;
    trans_flat = (uint8_t*)malloc(state_count * V * sizeof(uint8_t));
    trans_count = (uint8_t*)malloc(state_count * sizeof(uint8_t));
    for (idx_t p = 0; p < V; ++p) {
        for (idx_t q = 0; q < V; ++q) {
            size_t base = ((size_t)p * V + q) * V;
            uint8_t tcnt = 0;
            for (idx_t r = 0; r < V; ++r) {
                if (is_allowed_transition(valid_masks[p],
                                         valid_masks[q],
                                         valid_masks[r]))
                    trans_flat[base + tcnt++] = (uint8_t)r;
            }
            trans_count[(size_t)p * V + q] = tcnt;
        }
    }

    /* 3) Allocate DP arrays (state_count entries) */
    cur = (seq_t*)malloc(state_count * sizeof(seq_t));
    nxt = (seq_t*)malloc(state_count * sizeof(seq_t));

    /* 4) Initialise DP for n = 2 (all pairs allowed) */
    for (size_t i = 0; i < state_count; ++i) cur[i] = 1;

    /* 5) Initialise cache for a(1) and a(2) */
    cache_size = 3; /* indices 0,1,2 */
    cache = (seq_t*)calloc(cache_size, sizeof(seq_t));
    cache[1] = (seq_t)V;                    /* a(1) */
    cache[2] = (seq_t)V * (seq_t)V;          /* a(2) */
    cur_len = 2;
}

/* Ensure cache can hold up to index n */
static void extend_cache(idx_t n) {
    if (n < cache_size) return;
    size_t new_size = n + 1;
    cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    for (idx_t i = cache_size; i < new_size; ++i) cache[i] = 0;
    cache_size = new_size;
}

/* Compute values up to (and including) n */
static void compute_up_to(idx_t n) {
    if (!initialized) { init_sequence(); initialized = true; }
    if (cur_len >= n) return;             /* already computed */
    while (cur_len < n) {
        /* zero nxt array */
        for (size_t i = 0; i < state_count; ++i) nxt[i] = 0;

        /* propagate from cur to nxt using transitions */
        for (idx_t p = 0; p < V; ++p) {
            for (idx_t q = 0; q < V; ++q) {
                size_t idx = (size_t)p * V + q;
                seq_t val = cur[idx];
                if (val == 0) continue;
                size_t base = idx * V;
                uint8_t cnt = trans_count[idx];
                for (uint8_t k = 0; k < cnt; ++k) {
                    uint8_t r = trans_flat[base + k];
                    size_t dst = (size_t)q * V + r;
                    nxt[dst] += val;
                }
            }
        }

        /* sum all entries of nxt to get a(cur_len+1) */
        seq_t total = 0;
        for (size_t i = 0; i < state_count; ++i) total += nxt[i];
        extend_cache(cur_len + 1);
        cache[cur_len + 1] = total;

        /* swap cur and nxt for next iteration */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
        ++cur_len;
    }
}

/* Public function required by the exercise */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n = 0 */
    compute_up_to(n);
    return cache[n];
}
