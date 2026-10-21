/*
 * A234417  Number of (n+1)X(4+1) 0..7 arrays with every 2X2 subblock having
 *          the absolute values of all six edge and diagonal differences no
 *          larger than 1.
 * https://oeis.org/A234417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_seq_array, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A234417 – Number of (n+1)×5 arrays over 0..7
 * where every 2×2 subblock has all six edge and diagonal
 * differences ≤ 1.
 *
 * Implementation uses a row‑DP.  All admissible 5‑tuples are
 * enumerated; transitions between rows that satisfy the 2×2
 * constraint are precomputed.  For each n the number of valid
 * (n+1)-row arrays is the sum of the DP state after n transitions.
 *
 * The code stores intermediate results in seq_t (the big‑integer
 * type supplied by the build system) and memoises the sequence
 * values for arbitrary access patterns.
 *
 * No external libraries other than the provided utilities are used.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ===== Utility includes ===== */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) -> seq_t* */
#include "../utils/ensure_capacity.h"  /* ensure_capacity(size_t, size_t*, void**, size_t) */

seq_t a(idx_t n);

/* ===== Global state ===== */

/* Number of admissible rows */
static size_t NUM_ROWS = 0;

/* row_digits[row][col] – the 5 digits of a row */
static uint8_t (*row_digits)[5] = NULL;

/* Row transition data */
static size_t *row_adj_start = NULL;  /* start index in adj_data for each row */
static size_t *row_adj_len   = NULL;  /* number of neighbours for each row  */
static idx_t *adj_data       = NULL;  /* concatenated neighbour lists */

/* DP buffers: dp_cur holds counts for the current height,
 * dp_next will hold the next height.  Each element is a big integer. */
static seq_t *dp_cur  = NULL;
static seq_t *dp_next = NULL;

/* Sequence cache: a_cache[k] = a(k) for 1≤k<cap.  cap is the current allocation size. */
static seq_t *a_cache   = NULL;
static size_t cache_cap = 0;     /* allocation size of a_cache */
static idx_t  cache_max = 0;     /* maximum n already computed */

/* ===== Helper functions ===== */

/* Build the list of admissible rows and their adjacency graph. */
static void build_rows_and_transitions(void)
{
    /* Enumerate all 8⁵ = 32768 possible rows. */
    const idx_t MAXROW = (idx_t)1 << (3 * 5);   /* 8^5 = 32768 */
    idx_t *id_to_idx = (idx_t *)calloc(MAXROW, sizeof(idx_t));
    if (!id_to_idx) exit(EXIT_FAILURE);

    idx_t *valid_ids = (idx_t *)malloc(MAXROW * sizeof(idx_t));
    if (!valid_ids) exit(EXIT_FAILURE);
    idx_t valid_count = 0;

    /* Collect rows that satisfy adjacency restriction within the row. */
    for (idx_t id = 0; id < MAXROW; ++id) {
        uint8_t d[5];
        idx_t tmp = id;
        for (int k = 4; k >= 0; --k) {
            d[k] = tmp & 7;   /* modulo 8 */
            tmp >>= 3;        /* divide by 8 */
        }
        bool ok = true;
        for (int i = 0; i < 4 && ok; ++i)
            if (abs((int)d[i] - (int)d[i + 1]) > 1)
                ok = false;
        if (ok) {
            id_to_idx[id] = valid_count;
            valid_ids[valid_count++] = id;
        }
    }
    NUM_ROWS = valid_count;

    /* Store the digits of admissible rows. */
    row_digits = (uint8_t (*)[5])malloc(NUM_ROWS * 5 * sizeof(uint8_t));
    if (!row_digits) exit(EXIT_FAILURE);
    for (idx_t idx = 0; idx < NUM_ROWS; ++idx) {
        idx_t id = valid_ids[idx];
        idx_t tmp = id;
        for (int k = 4; k >= 0; --k) {
            row_digits[idx][k] = tmp & 7;
            tmp >>= 3;
        }
    }
    free(valid_ids);
    free(id_to_idx);

    /* Allocate transition data. */
    row_adj_start = (size_t *)malloc(NUM_ROWS * sizeof(size_t));
    row_adj_len   = (size_t *)malloc(NUM_ROWS * sizeof(size_t));
    if (!row_adj_start || !row_adj_len) exit(EXIT_FAILURE);

    size_t estimated_edges = NUM_ROWS * 8;  /* initial guess */
    adj_data = (idx_t *)malloc(estimated_edges * sizeof(idx_t));
    if (!adj_data) exit(EXIT_FAILURE);
    size_t edges_used = 0;

    /* For every pair of rows, test whether they can be adjacent. */
    for (idx_t a = 0; a < NUM_ROWS; ++a) {
        row_adj_start[a] = edges_used;
        for (idx_t b = 0; b < NUM_ROWS; ++b) {
            bool ok = true;
            for (int i = 0; i < 4 && ok; ++i) {
                uint8_t r0 = row_digits[a][i];
                uint8_t r1 = row_digits[a][i + 1];
                uint8_t c0 = row_digits[b][i];
                uint8_t c1 = row_digits[b][i + 1];
                uint8_t maxv = r0, minv = r0;
                if (r1 > maxv) maxv = r1; if (r1 < minv) minv = r1;
                if (c0 > maxv) maxv = c0; if (c0 < minv) minv = c0;
                if (c1 > maxv) maxv = c1; if (c1 < minv) minv = c1;
                if (maxv - minv > 1) ok = false;
            }
            if (ok) {
                if (edges_used == estimated_edges) {
                    estimated_edges <<= 1;
                    adj_data = (idx_t *)realloc(adj_data,
                                                estimated_edges * sizeof(idx_t));
                    if (!adj_data) exit(EXIT_FAILURE);
                }
                adj_data[edges_used++] = b;
            }
        }
        row_adj_len[a] = edges_used - row_adj_start[a];
    }
    /* shrink to actual size */
    adj_data = (idx_t *)realloc(adj_data, edges_used * sizeof(idx_t));
}

/* One‑time initialization of DP structures and cache. */
static void ensure_initialized(void)
{
    if (!dp_cur) {
        build_rows_and_transitions();

        dp_cur  = alloc_seq_array(NUM_ROWS);
        dp_next = alloc_seq_array(NUM_ROWS);
        if (!dp_cur || !dp_next) exit(EXIT_FAILURE);

        /* Start with a single row: one configuration per admissible row. */
        seq_t one = 1;
        for (size_t i = 0; i < NUM_ROWS; ++i)
            dp_cur[i] = one;

        /* Allocate cache for at least first 16 values. */
        cache_cap = 16;
        a_cache = alloc_seq_array(cache_cap);
        if (!a_cache) exit(EXIT_FAILURE);
    }
}

/* ===== Public API ===== */

seq_t a(idx_t n)
{
    /* a(1) corresponds to 2 rows, so n must be ≥ 1 */
    if (n < 1) return (seq_t)0;

    ensure_initialized();

    /* Grow cache if needed. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        ensure_capacity(new_cap, &cache_cap, (void **)&a_cache, sizeof(seq_t));
    }

    /* Return cached value if already computed. */
    if (n <= cache_max)
        return a_cache[n];

    /* Compute DP up to the requested n. */
    for (idx_t target = cache_max + 1; target <= n; ++target) {
        /* Clear the next layer. */
        memset(dp_next, 0, NUM_ROWS * sizeof(seq_t));

        /* Transition step. */
        for (idx_t a = 0; a < NUM_ROWS; ++a) {
            seq_t v = dp_cur[a];
            if (v == 0) continue;
            size_t start = row_adj_start[a];
            size_t len   = row_adj_len[a];
            for (size_t j = 0; j < len; ++j) {
                idx_t b = adj_data[start + j];
                dp_next[b] += v;
            }
        }

        /* Sum all states to obtain a(target). */
        seq_t total = 0;
        for (idx_t i = 0; i < NUM_ROWS; ++i)
            total += dp_next[i];
        a_cache[target] = total;

        /* Prepare for next iteration: swap buffers. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    cache_max = n;
    return a_cache[n];
}
