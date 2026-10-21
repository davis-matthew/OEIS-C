/*
 * A207883  Number of nX6 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   alloc_seq_array, bad_horizontal, bad_vertical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP over rows with forbidden horizontal patterns 000,101
   and forbidden vertical patterns 011,110 in a 0..1 array of width 6.  */
/* This implementation is O(n * 19²) and works for any seq_t type
   (fixed‑size unsigned or arbitrary‑precision class). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/bad_horizontal.h"
#include "../utils/bad_vertical.h"

/* Check that a 6‑bit row does not contain any forbidden horizontal window. */
static inline bool allowed_row(uint8_t r)
{
    for (int shift = 0; shift <= 3; ++shift) {
        if (bad_horizontal((r >> shift) & 0x7))
            return false;
    }
    return true;
}

/* ---------- global state ---------- */
static bool already_init = false;
static int  M = 0;                     /* number of allowed rows (19)   */
static uint8_t *allowed_rows = NULL;   /* M element array of row masks  */
static size_t state_cnt = 0;           /* M * M                     */
static uint64_t *adj = NULL;           /* transition masks per state  */

static seq_t *dp_cur  = NULL;          /* DP for current row      */
static seq_t *dp_nxt  = NULL;          /* DP for next row         */

static seq_t *cache      = NULL;       /* cached sequence values  */
static size_t cache_cap  = 0;          /* allocated cache size    */
static idx_t  max_n_done = 0;          /* largest n for which cache is valid */

/* ---------- initialization ---------- */
static void init(void)
{
    if (already_init) return;

    /* Enumerate allowed 6‑bit rows. */
    int tmp = 0;
    uint8_t rows[64];
    for (uint8_t r = 0; r < 64; ++r)
        if (allowed_row(r)) rows[tmp++] = r;
    M = tmp;
    allowed_rows = (uint8_t*)malloc(M * sizeof(uint8_t));
    memcpy(allowed_rows, rows, M * sizeof(uint8_t));

    /* Precompute transition masks. */
    state_cnt = (size_t)M * M;
    adj = (uint64_t*)malloc(state_cnt * sizeof(uint64_t));
    memset(adj, 0, state_cnt * sizeof(uint64_t));

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            uint64_t mask = 0;
            uint8_t row_i = allowed_rows[i];
            uint8_t row_j = allowed_rows[j];
            for (int k = 0; k < M; ++k) {
                uint8_t row_k = allowed_rows[k];
                bool ok = true;
                for (int col = 0; col < 6; ++col) {
                    uint8_t b1 = (row_i >> col) & 1;
                    uint8_t b2 = (row_j >> col) & 1;
                    uint8_t b3 = (row_k >> col) & 1;
                    if (bad_vertical(b1, b2, b3)) { ok = false; break; }
                }
                if (ok) mask |= (uint64_t)1 << k;
            }
            adj[(size_t)i * M + j] = mask;
        }
    }

    /* Allocate DP arrays (they are zeroed later). */
    dp_cur = alloc_seq_array(state_cnt);
    dp_nxt = alloc_seq_array(state_cnt);

    already_init = true;
}

/* ---------- enlarge cache if necessary ---------- */
static void allocate_cache(idx_t n)
{
    if (cache == NULL || (size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        /* realloc returns void*, cast explicitly for C++ */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* ---------- compute terms up to n ---------- */
static void compute_to(idx_t n)
{
    if (n <= max_n_done) return;          /* nothing to do */

    init();                                /* ensure global tables */

    allocate_cache(n);                     /* cache large enough */

    /* First terms: a(1) and a(2). */
    if (max_n_done == 0) {
        cache[0] = (seq_t)M;                 /* a(1) */
        max_n_done = 1;
        if (n == 1) return;
    }
    if (max_n_done == 1) {
        cache[1] = (seq_t)M * (seq_t)M;      /* a(2) */
        max_n_done = 2;

        /* initialise DP for 2 rows: one way for each allowed pair. */
        for (size_t s = 0; s < state_cnt; ++s)
            dp_cur[s] = (seq_t)1;
    }

    /* DP for rows 3..n. */
    for (idx_t row = max_n_done + 1; row <= n; ++row) {
        /* zero next DP array */
        memset(dp_nxt, 0, state_cnt * sizeof(seq_t));

        /* transition from each current state */
        for (size_t s = 0; s < state_cnt; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            uint64_t mask = adj[s];
            size_t j = s % M;                 /* second row index of state */
            while (mask) {
                int k = __builtin_ctzll(mask);
                size_t ns = j * M + (size_t)k;
                dp_nxt[ns] += val;
                mask &= mask - 1;
            }
        }

        /* sum all states => a(row) */
        seq_t tot = 0;
        for (size_t s = 0; s < state_cnt; ++s)
            tot += dp_nxt[s];
        cache[row - 1] = tot;

        /* swap pointers for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
    }

    max_n_done = n;
}

/* ---------- public interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    compute_to(n);
    return cache[n - 1];
}
