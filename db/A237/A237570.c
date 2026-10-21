/*
 * A237570  Number of (n+1)X(2+1) 0..3 arrays with the upper median minus the
 *          lower median of every 2X2 subblock differing from its horizontal
 *          and vertical neighbors by exactly one.
 * https://oeis.org/A237570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   alloc_seq_array, medianDiff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A237570: Number of (n+1)×3 arrays with the stated median‑difference constraints.
   Computed by dynamic programming on the graph of valid consecutive‑row pairs.
   Each vertex represents a 2×3 row (4‑tuple over {0,1,2,3}); an edge (u,v)
   represents two consecutive rows, storing the two 2×2 subblock median
   differences d0 and d1.  An edge is allowed from e1 to e2 iff
   |e1.d0‑e2.d0| = 1 and |e1.d1‑e2.d1| = 1.  The number a(n) is the number of
   length‑n paths in this graph (equivalent to counting the admissible
   arrays).  The algorithm runs in O(n·E) where E ≈ 1876, well within the
   time limit for n up to 10000. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/medianDiff.h"

/* Types supplied by the build system ----------------------------------- */
typedef uint64_t idx_t;         /* index type */

/* Edge representation ----------------------------------------------------- */
typedef struct {
    uint8_t from;   /* left row index (0..63) */
    uint8_t to;     /* right row index (0..63) */
    uint8_t d0;     /* median‑difference of column 0 */
    uint8_t d1;     /* median‑difference of column 1 */
} Edge;

/* Static state ------------------------------------------------------------ */
static int initialized = 0;

static Edge *edges = NULL;      /* all admissible edges */
static size_t E = 0;            /* number of edges */

static int *adjStart = NULL;    /* start index of each edge’s adjacency list in adjList */
static int *adjList  = NULL;    /* concatenated adjacency lists */

static seq_t *dp_cur  = NULL;   /* DP array for current step */
static seq_t *dp_next = NULL;   /* DP array for next step */

static seq_t *results   = NULL;  /* cached results a[1..] */
static idx_t  results_size = 0;
static idx_t  cached_max   = 0;

/* Row digit tables (0..63 expanded to three base‑4 digits) ----------------- */
static uint8_t row0[64];
static uint8_t row1[64];
static uint8_t row2[64];

/* Initialise graph, DP arrays, and first result -------------------------------- */
static void init(void)
{
    /* Pre‑compute row digit tables */
    for (int r = 0; r < 64; ++r) {
        row0[r] = (uint8_t)(r / 16);
        row1[r] = (uint8_t)((r / 4) % 4);
        row2[r] = (uint8_t)(r % 4);
    }

    /* Build admissible edges */
    edges = (Edge*)malloc(64 * 64 * sizeof(Edge));
    E = 0;
    for (uint8_t u = 0; u < 64; ++u) {
        for (uint8_t v = 0; v < 64; ++v) {
            uint8_t d0 = medianDiff(row0[u], row1[u], row0[v], row1[v]);
            uint8_t d1 = medianDiff(row1[u], row2[u], row1[v], row2[v]);
            if (abs((int)d0 - (int)d1) != 1) continue; /* horizontal rule */
            edges[E++] = (Edge){u, v, d0, d1};
        }
    }

    /* Compute adjacency lists */
    int *outCount = (int*)calloc(E, sizeof(int));
    for (size_t i = 0; i < E; ++i) {
        Edge *e1 = &edges[i];
        for (size_t j = 0; j < E; ++j) {
            Edge *e2 = &edges[j];
            if (e1->to != e2->from) continue;
            if (abs((int)e1->d0 - (int)e2->d0) != 1) continue;
            if (abs((int)e1->d1 - (int)e2->d1) != 1) continue;
            ++outCount[i];
        }
    }

    adjStart = (int*)malloc((E + 1) * sizeof(int));
    adjStart[0] = 0;
    for (size_t i = 0; i < E; ++i) {
        adjStart[i+1] = adjStart[i] + outCount[i];
    }
    int totalTrans = adjStart[E];
    adjList = (int*)malloc(totalTrans * sizeof(int));

    int *nextIdx = (int*)malloc(E * sizeof(int));
    memcpy(nextIdx, adjStart, E * sizeof(int));
    for (size_t i = 0; i < E; ++i) {
        Edge *e1 = &edges[i];
        for (size_t j = 0; j < E; ++j) {
            Edge *e2 = &edges[j];
            if (e1->to != e2->from) continue;
            if (abs((int)e1->d0 - (int)e2->d0) != 1) continue;
            if (abs((int)e1->d1 - (int)e2->d1) != 1) continue;
            adjList[nextIdx[i]++] = (int)j;
        }
    }
    free(nextIdx);
    free(outCount);

    /* Allocate DP arrays */
    dp_cur  = alloc_seq_array(E);
    dp_next = alloc_seq_array(E);

    /* Allocate results array for n=1 */
    results = alloc_seq_array(2);  /* at least size 2 (index 0 unused) */
    results_size = 2;

    /* Path length 1: each edge counts once */
    for (size_t i = 0; i < E; ++i) dp_cur[i] = 1;
    seq_t sum = 0;
    for (size_t i = 0; i < E; ++i) sum += dp_cur[i];
    results[1] = sum;
    cached_max = 1;

    initialized = 1;
}

/* Ensure that results[1..n] are computed ---------------------------------- */
static void ensure_up_to(idx_t n)
{
    if (!initialized) init();
    if (n <= cached_max) return;

    /* Resize results array if necessary */
    if (results_size <= n) {
        seq_t *newRes = alloc_seq_array(n + 1);
        for (idx_t i = 0; i <= cached_max; ++i) newRes[i] = results[i];
        results = newRes;
        results_size = n + 1;
    }

    for (idx_t step = cached_max + 1; step <= n; ++step) {
        /* zero dp_next */
        for (size_t e = 0; e < E; ++e) dp_next[e] = 0;

        /* transition */
        for (size_t e = 0; e < E; ++e) {
            seq_t val = dp_cur[e];
            if (val == 0) continue;
            int start = adjStart[e];
            int end   = adjStart[e + 1];
            for (int k = start; k < end; ++k) {
                int e2 = adjList[k];
                dp_next[e2] += val;
            }
        }

        /* sum dp_next to get a(step) */
        seq_t s = 0;
        for (size_t e = 0; e < E; ++e) s += dp_next[e];
        results[step] = s;

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }
    cached_max = n;
}

/* Public function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_up_to(n);
    return results[n];
}
