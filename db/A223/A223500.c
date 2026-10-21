/*
 * A223500  Petersen graph (3,1) coloring a rectangular array: number of nX4
 *          0..5 arrays where 0..5 label nodes of a graph with edges 0,1 0,3
 *          3,5 3,4 1,2 1,4 4,5 2,0 2,5 and every array movement to a
 *          horizontal, diagonal or antidiagonal neighbor moves along an edge
 *          of this graph, with the array starting at 0.
 * https://oeis.org/A223500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A223500
 * Petersen graph (3,1) coloring of an n×4 array.
 *
 * The algorithm enumerates all valid 4‑cell rows (horizontal edges only),
 * precomputes for each row the list of rows that can follow it
 * (diagonal edge constraints only), and then uses dynamic programming
 * to extend the array row by row.  The sequence values are memoised,
 * so calls with arbitrary n are served in O(n) time from the highest
 * previously computed index.
 *
 * This implementation uses the interface type seq_t and idx_t supplied
 * by the build; all arithmetic is performed in seq_t to preserve
 * precision.  No external utilities are required.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*======================================================================
 *  Static data that is initialised on first use
 *====================================================================*/

static int ready = 0;           /* true once structures are built */

static uint8_t adj[6][6] = {0}; /* adjacency of the Petersen graph */

/* a 4‑cell state */
typedef struct { uint8_t v[4]; } state_t;

/* list of all admissible states (horizontal edges ok)          */
static state_t *states = NULL;
static size_t   state_cnt = 0;

/* indices of states that start with 0 in column 0 (a(1) uses them) */
static size_t *init_idx = NULL;
static size_t   init_cnt = 0;

/* for each state i, compat[i] is an array of indices j such that
   state j can follow state i in the next row (diagonal edges).       */
static size_t **compat     = NULL;
static size_t *compat_cnt = NULL;

/* DP temporaries: dpCurr holds counts for the current row height,
   dpNext holds counts for the next row height.                       */
static seq_t *dpCurr = NULL;
static seq_t *dpNext = NULL;

/* memoised sequence values: cache[n-1] is a(n) for n>=1            */
static seq_t *cache = NULL;
static idx_t  cache_cap   = 0; /* allocated capacity (values) */
static idx_t  cache_size  = 0; /* number of stored values      */
static idx_t  computedN   = 0; /* highest n for which cache is valid */

/*======================================================================
 *  Helper: grow cache to hold at least 'need' elements
 *====================================================================*/
static void ensure_cache_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) abort();            /* fatal error */
    cache = newp;
    cache_cap = new_cap;
}

/*======================================================================
 *  Initialise the static structures on first use
 *====================================================================*/
static void init_structures(void)
{
    /* Petersen graph adjacency table */
    const int edges[9][2] = {
        {0,1},{0,3},{3,5},{3,4},
        {1,2},{1,4},{4,5},{2,0},{2,5}
    };
    for (int i = 0; i < 9; ++i) {
        int u = edges[i][0], v = edges[i][1];
        adj[u][v] = adj[v][u] = 1;
    }

    /* generate all admissible rows (horizontal edges) */
    states = (state_t *)malloc(1296 * sizeof(state_t));   /* 6^4 */
    if (!states) abort();
    init_idx = (size_t *)malloc(1296 * sizeof(size_t));
    if (!init_idx) abort();

    size_t s = 0, i0 = 0;
    for (int a = 0; a < 6; ++a)
    for (int b = 0; b < 6; ++b)
    for (int c = 0; c < 6; ++c)
    for (int d = 0; d < 6; ++d) {
        if (!adj[a][b] || !adj[b][c] || !adj[c][d]) continue;
        states[s].v[0] = (uint8_t)a;
        states[s].v[1] = (uint8_t)b;
        states[s].v[2] = (uint8_t)c;
        states[s].v[3] = (uint8_t)d;
        if (a == 0) init_idx[i0++] = s;
        ++s;
    }
    state_cnt = s;
    init_cnt  = i0;

    /* allocate compatibility lists */
    compat     = (size_t **)malloc(state_cnt * sizeof(size_t *));
    compat_cnt = (size_t *)malloc(state_cnt * sizeof(size_t));
    if (!compat || !compat_cnt) abort();
    for (size_t i = 0; i < state_cnt; ++i) {
        compat[i]     = (size_t *)malloc(state_cnt * sizeof(size_t));
        compat_cnt[i] = 0;
        if (!compat[i]) abort();
    }

    /* compute compatibility between rows
       (only diagonal edges between adjacent rows) */
    for (size_t i = 0; i < state_cnt; ++i) {
        const state_t *si = &states[i];
        for (size_t j = 0; j < state_cnt; ++j) {
            const state_t *sj = &states[j];
            int ok = 1;
            for (int k = 0; k < 4 && ok; ++k) {
                if (k > 0 && !adj[si->v[k-1]][sj->v[k]]) ok = 0;
                if (!ok) break;
                if (k < 3 && !adj[si->v[k+1]][sj->v[k]]) ok = 0;
            }
            if (ok) compat[i][compat_cnt[i]++] = j;
        }
    }

    /* DP temporary arrays */
    dpCurr = (seq_t *)malloc(state_cnt * sizeof(seq_t));
    dpNext = (seq_t *)malloc(state_cnt * sizeof(seq_t));
    if (!dpCurr || !dpNext) abort();

    ready = 1;
}

/*======================================================================
 *  Sequence function
 *====================================================================*/
seq_t a(idx_t n)
{
    if (!ready) init_structures();

    if (n == 0) abort();           /* sequence starts at index 1 */

    /* compute any missing values up to n */
    if (n > cache_size) {
        /* initialise DP for height 1 if this is the very first call */
        if (computedN == 0) {
            memset(dpCurr, 0, state_cnt * sizeof(seq_t));
            for (size_t idx = 0; idx < init_cnt; ++idx)
                dpCurr[ init_idx[idx] ] = (seq_t)1;

            seq_t sum = 0;
            for (size_t i = 0; i < state_cnt; ++i) sum += dpCurr[i];

            ensure_cache_capacity(1);
            cache[0] = sum;
            cache_size = 1;
            computedN  = 1;
        }

        /* extend DP step by step */
        for (idx_t cur = computedN + 1; cur <= n; ++cur) {
            memset(dpNext, 0, state_cnt * sizeof(seq_t));

            for (size_t i = 0; i < state_cnt; ++i) {
                seq_t val = dpCurr[i];
                if (!val) continue;
                size_t *list = compat[i];
                size_t cnt   = compat_cnt[i];
                for (size_t t = 0; t < cnt; ++t)
                    dpNext[ list[t] ] += val;
            }

            /* swap */
            seq_t *tmp = dpCurr;
            dpCurr = dpNext;
            dpNext = tmp;

            /* compute total for this height */
            seq_t total = 0;
            for (size_t i = 0; i < state_cnt; ++i)
                total += dpCurr[i];

            ensure_cache_capacity(cur);
            cache[cur-1] = total;
        }
        cache_size = n;
        computedN  = n;
    }

    return cache[n-1];
}
