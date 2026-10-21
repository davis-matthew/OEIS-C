/*
 * A223338  5 X 5 X 5 triangular graph coloring a rectangular array: number of
 *          n X 2 0..14 arrays where 0..14 label nodes of a graph with edges
 *          0,1 0,2 1,2 1,3 1,4 2,4 3,4 2,5 4,5 3,6 3,7 4,7 6,7 4,8 5,8 7,8
 *          5,9 8,9 6,10 6,11 7,11 10,11 7,12 8,12 11,12 11,12 8,13 9,13 12,13
 *          9,14 13,14 and every array movemen...
 * https://oeis.org/A223338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A223338.
   The value is the number of proper labelings of an n×2 array with
   labels from {0,…,14} such that every horizontal or vertical step
   follows an edge of the fixed graph.  The algorithm performs
   dynamic programming over walks of length n‑1 in the
   directed‑edge automaton of the graph; an initialization step
   sets a(1)=60.  The function is memoized and runs in O(n) time
   per call (and O(1) memory beyond the small 60‑state automaton). */

#include <stdlib.h>
#include <stdint.h>

static const int EDGE_LIST[][2] = {
    {0,1},{0,2},{1,2},{1,3},{1,4},{2,4},{3,4},
    {2,5},{4,5},{3,6},{3,7},{4,7},{6,7},{4,8},{5,8},
    {7,8},{5,9},{8,9},{6,10},{6,11},{7,11},{10,11},
    {7,12},{8,12},{11,12},{8,13},{9,13},{12,13},
    {9,14},{13,14}
};
static const int NUM_EDGES = sizeof(EDGE_LIST)/sizeof(EDGE_LIST[0]);

#define MAX_V 15
#define MAX_STATES 60
static unsigned char graph_adj[MAX_V][MAX_V];
static int state_idx[MAX_V][MAX_V];
static int state_u[MAX_STATES];
static int state_v[MAX_STATES];
static int trans[MAX_STATES][256];
static unsigned char trans_len[MAX_STATES];
static int num_states;

/* Cache of sequence values.  cache[i] contains a(i+1). */
static seq_t *cache = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Counts for each state at the current length. */
static seq_t state_count[MAX_STATES];
static seq_t next_count[MAX_STATES];
static idx_t current_len = 0;       /* last computed length */

/* Grow the cache array up to at least n elements. */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < (size_t)n) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);          /* allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

/* One‑time initialization of graph data and state machine. */
static void init(void)
{
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    /* Build adjacency matrix for the underlying graph. */
    for (int i = 0; i < MAX_V; ++i)
        for (int j = 0; j < MAX_V; ++j)
            graph_adj[i][j] = 0;
    for (int e = 0; e < NUM_EDGES; ++e) {
        int u = EDGE_LIST[e][0];
        int v = EDGE_LIST[e][1];
        graph_adj[u][v] = graph_adj[v][u] = 1;
    }

    /* Enumerate ordered edges as states. */
    for (int i = 0; i < MAX_V; ++i)
        for (int j = 0; j < MAX_V; ++j)
            state_idx[i][j] = -1;
    num_states = 0;
    for (int u = 0; u < MAX_V; ++u)
        for (int v = 0; v < MAX_V; ++v)
            if (graph_adj[u][v]) {
                state_u[num_states] = u;
                state_v[num_states] = v;
                state_idx[u][v] = num_states;
                ++num_states;
            }

    /* Build transition lists. */
    for (int s = 0; s < num_states; ++s) {
        int a = state_u[s];
        int b = state_v[s];
        int *tr = trans[s];
        unsigned char *lenp = &trans_len[s];
        *lenp = 0;
        for (int a2 = 0; a2 < MAX_V; ++a2)
            if (graph_adj[a][a2])
                for (int b2 = 0; b2 < MAX_V; ++b2)
                    if (graph_adj[b][b2]) {
                        int t = state_idx[a2][b2];
                        tr[(*lenp)++] = t;
                    }
    }

    /* Initialise counts for length 1: each state has count 1. */
    for (int s = 0; s < num_states; ++s)
        state_count[s] = (seq_t)1;

    /* Cache first term a(1) = number of directed edges = num_states. */
    ensure_cache_size(1);
    cache[0] = (seq_t)num_states;
    cache_len = 1;
    current_len = 1;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init();

    if ((size_t)n > cache_len) {
        ensure_cache_size(n);
        /* Compute terms from current_len+1 up to n. */
        for (idx_t len = current_len + 1; len <= n; ++len) {
            /* zero next_count */
            for (int s = 0; s < num_states; ++s) next_count[s] = (seq_t)0;
            /* propagate counts through transitions */
            for (int s = 0; s < num_states; ++s) {
                seq_t cnt = state_count[s];
                if (cnt == (seq_t)0) continue;
                int *tlist = trans[s];
                unsigned char tlen = trans_len[s];
                for (int i = 0; i < tlen; ++i)
                    next_count[tlist[i]] += cnt;
            }
            /* copy back and sum total */
            seq_t total = (seq_t)0;
            for (int s = 0; s < num_states; ++s) {
                state_count[s] = next_count[s];
                total += next_count[s];
            }
            cache[len-1] = total;
        }
        cache_len = n;
        current_len = n;
    }
    return cache[n-1];
}
