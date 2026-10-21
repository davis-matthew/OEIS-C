/*
 * A223251  Two-loop graph coloring a rectangular array: number of n X 4 0..4
 *          arrays where 0..4 label nodes of a graph with edges 0,1 1,2 2,0
 *          0,3 3,4 4,0 and every array movement to a horizontal or vertical
 *          neighbor moves along an edge of this graph.
 * https://oeis.org/A223251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 arrays over {0,1,2,3,4} where every
 *            horizontal or vertical step follows an edge of the graph: edges
 *            0–1, 1–2, 2–0, 0–3, 3–4, 4–0. Computed by dynamic programming
 *            over row states
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Adjacent matrix for the graph (rows, columns). */
static const unsigned char adj_matrix[5][5] = {
    {0,1,1,1,1},
    {1,0,1,0,0},
    {1,1,0,0,0},
    {1,0,0,0,1},
    {1,0,0,1,0}
};

/* Maximum possible states: 5^4 = 625 */
#define MAX_STATES 625
static uint8_t rows[MAX_STATES][4];
static size_t rows_count = 0;
static unsigned char trans[MAX_STATES][MAX_STATES];   /* transition matrix */

/* Dynamic programming arrays */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* Memoisation cache for a(n) */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* allocated size */
static idx_t cache_max = 0;      /* highest computed n */

/* Flag to ensure one-time initialisation */
static bool init_done = false;

/* Ensure the cache can hold up to needed elements. */
static void ensure_cache_capacity(idx_t needed)
{
    if ((size_t)needed <= cache_size) return;
    size_t newsize = (size_t)needed;
    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp) abort();
    /* initialise new entries to zero */
    for (size_t i = cache_size; i < newsize; ++i) tmp[i] = 0;
    cache = tmp;
    cache_size = newsize;
}

/* Build all row states and transition matrix.  Called once. */
static void init_sequence(void)
{
    if (init_done) return;
    init_done = true;

    /* Build valid row states (satisfying horizontal adjacency). */
    for (int c0 = 0; c0 < 5; ++c0) {
        for (int c1 = 0; c1 < 5; ++c1) if (adj_matrix[c0][c1]) {
            for (int c2 = 0; c2 < 5; ++c2) if (adj_matrix[c1][c2]) {
                for (int c3 = 0; c3 < 5; ++c3) if (adj_matrix[c2][c3]) {
                    rows[rows_count][0] = (uint8_t)c0;
                    rows[rows_count][1] = (uint8_t)c1;
                    rows[rows_count][2] = (uint8_t)c2;
                    rows[rows_count][3] = (uint8_t)c3;
                    ++rows_count;
                }
            }
        }
    }

    /* Build transition matrix between rows. */
    for (size_t i = 0; i < rows_count; ++i) {
        for (size_t j = 0; j < rows_count; ++j) {
            unsigned char ok = 1;
            for (int col = 0; col < 4; ++col) {
                if (!adj_matrix[rows[i][col]][rows[j][col]]) { ok = 0; break; }
            }
            trans[i][j] = ok;
        }
    }

    /* Allocate DP arrays. */
    dp_cur = (seq_t *)malloc(rows_count * sizeof(seq_t));
    dp_next = (seq_t *)malloc(rows_count * sizeof(seq_t));
    if (!dp_cur || !dp_next) abort();

    /* For n = 1, every row state is allowed. */
    for (size_t i = 0; i < rows_count; ++i) dp_cur[i] = (seq_t)1;

    /* Initialise cache with a(1). */
    cache = (seq_t *)malloc(sizeof(seq_t));
    if (!cache) abort();
    cache_size = 1;
    cache[0] = (seq_t)rows_count;   /* a(1) = number of valid row states */
    cache_max = 1;
}

/* Main function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */
    init_sequence();

    if (n <= cache_max) return cache[n - 1];

    /* Extend DP to reach desired n. */
    for (idx_t cur = cache_max + 1; cur <= n; ++cur) {
        /* Zero dp_next. */
        for (size_t j = 0; j < rows_count; ++j) dp_next[j] = 0;

        /* Transition from dp_cur to dp_next. */
        for (size_t p = 0; p < rows_count; ++p) {
            seq_t val = dp_cur[p];
            if (val == 0) continue;
            for (size_t q = 0; q < rows_count; ++q) {
                if (trans[p][q]) dp_next[q] += val;
            }
        }

        /* Sum over dp_next to get a(cur). */
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j < rows_count; ++j) sum += dp_next[j];

        /* Store in cache. */
        ensure_cache_capacity(cur);
        cache[cur - 1] = sum;
        cache_max = cur;

        /* Prepare for next iteration. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    return cache[n - 1];
}
