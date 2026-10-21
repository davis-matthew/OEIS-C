/*
 * A237085  Number of (n+1)X(3+1) 0..3 arrays with the maximum plus the upper
 *          median plus the lower median minus the minimum of every 2X2
 *          subblock equal.
 * https://oeis.org/A237085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   compute_K
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming over 256 possible rows.
   For each pair of consecutive rows compute the unique constant  
   K = sum(row1)+sum(row2) - 2*min(row1,row2) for every 2x2 block.  
   All three 2x2 blocks in a pair must give the same K (K in 0..12).  
   The DP enumerates all paths of length n through the 256-row graph  
   respecting a uniform K; the number of such paths equals a(n).   */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/compute_K.h"

typedef struct {
    uint16_t u, v;
} Edge;

typedef struct {
    Edge *data;
    size_t size;
    size_t cap;
} EdgeList;

static EdgeList edges_by_K[13];
static int edges_built = 0;

/* --------------------------------------------------------------- */
static void add_edge(int K, uint16_t u, uint16_t v)
{
    EdgeList *L = &edges_by_K[K];
    if (L->size == L->cap) {
        size_t new_cap = L->cap ? L->cap << 1 : 16;
        Edge *tmp = (Edge *)realloc(L->data, new_cap * sizeof(Edge));
        if (!tmp) abort();
        L->data = tmp;
        L->cap = new_cap;
    }
    L->data[L->size].u = u;
    L->data[L->size].v = v;
    ++L->size;
}

/* --------------------------------------------------------------- */
static uint8_t row_vals[256][4];
static int rows_built = 0;

static void build_rows(void)
{
    if (rows_built) return;
    for (uint16_t n = 0; n < 256; ++n) {
        uint16_t tmp = n;
        for (int j = 0; j < 4; ++j) {
            row_vals[n][j] = tmp & 3;
            tmp >>= 2;
        }
    }
    rows_built = 1;
}

/* --------------------------------------------------------------- */
static void build_edges(void)
{
    if (edges_built) return;
    build_rows();
    for (int K = 0; K < 13; ++K) {
        edges_by_K[K].data = NULL;
        edges_by_K[K].size = edges_by_K[K].cap = 0;
    }

    for (uint16_t u = 0; u < 256; ++u) {
        const uint8_t *au = row_vals[u];
        for (uint16_t v = 0; v < 256; ++v) {
            const uint8_t *av = row_vals[v];
            int K = compute_K(au, av);
            if (K >= 0)
                add_edge(K, u, v);
        }
    }
    edges_built = 1;
}

/* --------------------------------------------------------------- */
static seq_t dpA[256][13];
static seq_t dpB[256][13];
static seq_t (*dp_prev)[13] = dpA;
static seq_t (*dp_cur)[13]  = dpB;

/* --------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* --------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;   /* dummy, a(0) */
        cache_size = 1;
    }
    build_edges();

    while (cache_size <= n) {
        size_t curr = cache_size;       /* compute a(curr) */

        /* clear dp_cur */
        for (int i = 0; i < 256; ++i)
            for (int k = 0; k < 13; ++k)
                dp_cur[i][k] = 0;

        if (curr == 1) {
            /* first transition */
            for (int K = 0; K < 13; ++K) {
                EdgeList *L = &edges_by_K[K];
                for (size_t idx = 0; idx < L->size; ++idx) {
                    uint16_t v = L->data[idx].v;
                    dp_cur[v][K] += (seq_t)1;
                }
            }
        } else {
            /* subsequent transitions */
            for (int K = 0; K < 13; ++K) {
                EdgeList *L = &edges_by_K[K];
                for (size_t idx = 0; idx < L->size; ++idx) {
                    uint16_t u = L->data[idx].u;
                    uint16_t v = L->data[idx].v;
                    const seq_t val = dp_prev[u][K];
                    if (val != 0)
                        dp_cur[v][K] += val;
                }
            }
        }

        /* compute total of all dp_cur entries = a(curr) */
        seq_t total = 0;
        for (int i = 0; i < 256; ++i)
            for (int k = 0; k < 13; ++k)
                total += dp_cur[i][k];

        /* append to cache */
        if (cache_size == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }
        cache[cache_size] = total;
        ++cache_size;

        /* swap dp_prev and dp_cur for next iteration */
        seq_t (*tmp)[13] = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;
    }
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* a(0) unused */
    ensure_cache(n);
    return cache[n];
}
