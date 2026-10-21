/*
 * A234817  Number of (n+1) X (2+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 6
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 13, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A234817 – Number of (n+1)×3 arrays over 0..6 with every
    2×2 subblock having its diagonal sum differing from its
    antidiagonal sum by 6 (absolute value 6).  

    The sequence is the number of walks of length n on a graph with
    343 vertices (each vertex represents a possible 3‑tuple of digits
    0..6).  An edge i→j exists iff the two consecutive rows
    represented by i and j satisfy the 2×2 block condition.
    Summing the number of walks of length n over all start and end
    vertices yields a(n).  

    Initial values:  a(1)=686, a(2)=2220, a(3)=6282, … (see OEIS)
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* 7³ possible rows. */
#define NODES 343

/* ------------------------------------------------------------ */
/* Adjacency list for the row‑graph.                         */
/* ------------------------------------------------------------ */
typedef struct {
    uint16_t *list;    /* indices of adjacent vertices */
    size_t   len;      /* number of entries in list   */
    size_t   cap;      /* allocated capacity          */
} AdjNode;

static AdjNode adj[NODES];

/* Append neighbour j to node i. */
static void push_adj(size_t i, uint16_t j)
{
    AdjNode *on = &adj[i];
    if (on->len == on->cap) {
        size_t new_cap = on->cap ? on->cap * 2 : 8;
        uint16_t *new_list =
            (uint16_t *)realloc(on->list, new_cap * sizeof(uint16_t));
        if (!new_list) abort();
        on->list = new_list;
        on->cap  = new_cap;
    }
    on->list[on->len++] = j;
}

/* Build the graph: an edge i→j exists iff all 2×2 blocks between
   the rows represented by i and j satisfy the ±6 condition. */
static void build_graph(void)
{
    int r1[ NODES ], r2[ NODES ], r3[ NODES ];

    for (size_t i = 0; i < NODES; ++i) {
        r1[i] = (int)(i / 49);          /* most significant digit */
        r2[i] = (int)((i / 7) % 7);
        r3[i] = (int)(i % 7);           /* least significant      */
    }

    for (size_t i = 0; i < NODES; ++i) {
        for (size_t j = 0; j < NODES; ++j) {
            if (i == j) continue;
            int d1 = r1[i] + r2[j] - r2[i] - r1[j];
            if (d1 != 6 && d1 != -6) continue;
            int d2 = r2[i] + r3[j] - r3[i] - r2[j];
            if (d2 != 6 && d2 != -6) continue;
            push_adj(i, (uint16_t)j);
        }
    }
}

/* ------------------------------------------------------------ */
/* Dynamic programming data.                                   */
/* ------------------------------------------------------------ */
static seq_t *dp_prev = NULL;   /* counts for step t‑1   */
static seq_t *dp_curr = NULL;   /* counts for step t     */

/* ------------------------------------------------------------ */
/* Cache for computed a(n).                                    */
/* ------------------------------------------------------------ */
static seq_t *cache      = NULL;   /* 1‑based: cache[1] = a(1) */
static size_t cache_size = 0;      /* allocated size (incl. 0) */
static size_t max_n_cached = 0;    /* highest n already computed */

/* ------------------------------------------------------------ */
/* One–time initialisation.                                    */
/* ------------------------------------------------------------ */
static void init(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Initialise adjacency structure. */
    for (size_t i = 0; i < NODES; ++i) {
        adj[i].list = NULL;
        adj[i].len  = 0;
        adj[i].cap  = 0;
    }
    build_graph();

    /* Allocate DP arrays. */
    dp_prev = (seq_t *)malloc(NODES * sizeof(seq_t));
    dp_curr = (seq_t *)malloc(NODES * sizeof(seq_t));
    if (!dp_prev || !dp_curr) abort();

    /* Base state: first row may be any of the NODES possibilities. */
    for (size_t i = 0; i < NODES; ++i) dp_prev[i] = (seq_t)1;
    for (size_t i = 0; i < NODES; ++i) dp_curr[i] = (seq_t)0;

    /* Allocate cache for at least index 1. */
    cache = (seq_t *)malloc(2 * sizeof(seq_t));   /* indices 0 and 1 */
    if (!cache) abort();
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)0;
    cache_size = 2;
    max_n_cached = 0;
}

/* ------------------------------------------------------------ */
/* Sequence function a(n).                                      */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside defined range */

    init();

    /* Grow cache if necessary. */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        /* initialise new cells to 0 */
        for (size_t i = cache_size; i < new_size; ++i)
            new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute terms up to the requested n. */
    while (max_n_cached < (size_t)n) {
        /* Zero dp_curr for this step. */
        for (size_t i = 0; i < NODES; ++i) dp_curr[i] = (seq_t)0;

        /* Propagate counts to adjacent rows. */
        for (size_t i = 0; i < NODES; ++i) {
            seq_t val = dp_prev[i];
            if (val == (seq_t)0) continue;
            AdjNode *node = &adj[i];
            for (size_t k = 0; k < node->len; ++k) {
                uint16_t j = node->list[k];
                dp_curr[j] += val;
            }
        }

        /* Sum to obtain a(max_n_cached+1). */
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < NODES; ++i) sum += dp_curr[i];
        cache[++max_n_cached] = sum;

        /* Swap dp_prev and dp_curr for next iteration. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    return cache[(size_t)n];
}
