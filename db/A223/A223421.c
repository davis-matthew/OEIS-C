/*
 * A223421  3-level binary fanout graph coloring a rectangular array: number
 *          of nX6 0..6 arrays where 0..6 label nodes of a graph with edges
 *          0,1 1,3 1,4 0,2 2,5 2,6 and every array movement to a horizontal
 *          or vertical neighbor moves along an edge of this graph.
 * https://oeis.org/A223421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 arrays of labels 0..6 * where adjacent
 *            cells (horizontal or vertical) are connected * by the edges
 *            0‑1,0‑2,1‑3,1‑4,2‑5,2‑6 of the graph. * * The implementation
 *            uses dynamic programming over all * possible rows of length 6
 *            that satisfy the horizontal * constraints. For each ro...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The build supplies seq_t and idx_t; we use them directly. */

/* ----------------------------------------------------------------
 *  Internal constants and structures
 * ---------------------------------------------------------------- */
#define MAX_STATE 200          /* enough for the 192 valid rows */
#define MAX_COMPAT 730         /* 3^6 = 729 possible rows */

typedef struct {
    uint8_t node[6];
} Row;

static Row rows[MAX_STATE];
static int state_count = 0;      /* number of generated rows */

static int neigh_cnt[7];
static int neigh[7][3];

static int compat[MAX_STATE][MAX_COMPAT];
static int compat_cnt[MAX_STATE];

static seq_t *dp_curr = NULL;
static seq_t *dp_next = NULL;

static seq_t *a_cache = NULL;
static idx_t   a_cache_cap = 0;
static idx_t   last_n_computed = 0;

/* ----------------------------------------------------------------
 *  Helper functions
 * ---------------------------------------------------------------- */

/* Recursive generation of all legal rows */
static void dfs_generate(int pos, uint8_t *cur)
{
    if (pos == 6) {
        rows[state_count] = (Row){0,0,0,0,0,0};
        for (int i = 0; i < 6; ++i) rows[state_count].node[i] = cur[i];
        ++state_count;
        return;
    }
    if (pos == 0) {
        for (int v = 0; v <= 6; ++v) {
            cur[0] = (uint8_t)v;
            dfs_generate(1, cur);
        }
    } else {
        int prev = cur[pos-1];
        for (int k = 0; k < neigh_cnt[prev]; ++k) {
            cur[pos] = (uint8_t)neigh[prev][k];
            dfs_generate(pos+1, cur);
        }
    }
}

/* One‑time initialization of all data structures */
static bool initialized = false;
static void init(void)
{
    if (initialized) return;

    /* adjacency list of the graph */
    neigh_cnt[0] = 2; neigh[0][0] = 1; neigh[0][1] = 2;
    neigh_cnt[1] = 3; neigh[1][0] = 0; neigh[1][1] = 3; neigh[1][2] = 4;
    neigh_cnt[2] = 3; neigh[2][0] = 0; neigh[2][1] = 5; neigh[2][2] = 6;
    neigh_cnt[3] = 1; neigh[3][0] = 1;
    neigh_cnt[4] = 1; neigh[4][0] = 1;
    neigh_cnt[5] = 1; neigh[5][0] = 2;
    neigh_cnt[6] = 1; neigh[6][0] = 2;

    /* generate all rows */
    uint8_t tmp[6];
    dfs_generate(0, tmp);

    /* precompute compatibility lists */
    for (int i = 0; i < state_count; ++i) {
        compat_cnt[i] = 0;
        for (int j = 0; j < state_count; ++j) {
            bool ok = true;
            for (int p = 0; p < 6; ++p) {
                int a = rows[i].node[p];
                int b = rows[j].node[p];
                bool adj = false;
                for (int k = 0; k < neigh_cnt[a]; ++k)
                    if (neigh[a][k] == b) { adj = true; break; }
                if (!adj) { ok = false; break; }
            }
            if (ok) compat[i][compat_cnt[i]++] = j;
        }
    }

    /* allocate DP arrays */
    dp_curr = (seq_t*)malloc(state_count * sizeof(seq_t));
    dp_next = (seq_t*)malloc(state_count * sizeof(seq_t));
    for (int i = 0; i < state_count; ++i) {
        dp_curr[i] = 1;  /* one way to start with this row */
        dp_next[i] = 0;
    }

    /* initialize cache with a(1) */
    a_cache_cap = 2;
    a_cache = (seq_t*)malloc(a_cache_cap * sizeof(seq_t));
    a_cache[0] = 0;
    a_cache[1] = (seq_t)state_count;  /* a(1) = number of rows = 192 */
    last_n_computed = 1;

    initialized = true;
}

/* Ensure cache has space for index n */
static void ensure_cache_cap(idx_t n)
{
    if (n + 1 <= a_cache_cap) return;
    idx_t new_cap = a_cache_cap * 2;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    for (idx_t i = a_cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    a_cache = new_cache;
    a_cache_cap = new_cap;
}

/* ----------------------------------------------------------------
 *  Public function
 * ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init();                         /* one‑time setup */

    if (n == 0) return 0;           /* offset starts at 1 */

    if (n <= last_n_computed)       /* already computed */
        return a_cache[n];

    ensure_cache_cap(n);

    /* compute successive terms up to n */
    for (idx_t k = last_n_computed + 1; k <= n; ++k) {
        /* reset dp_next */
        for (int i = 0; i < state_count; ++i) dp_next[i] = 0;

        /* transition */
        for (int i = 0; i < state_count; ++i) {
            seq_t val = dp_curr[i];
            if (val == 0) continue;
            for (int t = 0; t < compat_cnt[i]; ++t) {
                int j = compat[i][t];
                dp_next[j] += val;
            }
        }

        /* sum all possibilities for current height */
        seq_t sum = 0;
        for (int i = 0; i < state_count; ++i) sum += dp_next[i];
        a_cache[k] = sum;

        /* swap dp arrays for next iteration */
        seq_t *tmp = dp_curr;
        dp_curr = dp_next;
        dp_next = tmp;
    }

    last_n_computed = n;
    return a_cache[n];
}
