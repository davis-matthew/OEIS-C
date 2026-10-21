/*
 * A205896  Number of 4X(n+1) 0..2 arrays with 2X2 subblock determinants
 *          differing from horizontal neighbors and permanents differing from
 *          vertical neighbors.
 * https://oeis.org/A205896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS sequence A205896 – number of 4×(n+1) 0..2 arrays with
 * 2×2 subblock determinants differing from horizontal neighbours
 * and permanents differing from vertical neighbours.
 * 
 * Algorithm:  model all possible adjacent column pairs (4 digits each)
 * as nodes of a directed graph.  An edge from node U to node V
 * exists iff the two 2×2 subblocks that overlap in the middle
 * satisfy the determinant inequality rule.  The total number of
 * arrays of width n+1 is the number of directed paths of length n
 * in this graph.  We compute all values by dynamic programming
 * over the graph, storing intermediate results in static buffers
 * and memoising already computed terms.
 */

/* --------------------------------------------------------------------- */
/* includes                                                       */
/* --------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Types supplied externally: seq_t – integer type of arbitrary size   */
/*                                                idx_t – index, 64‑bit       */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Constants – fixed size of the matrix (rows = 4, symbols 0..2)       */
/* --------------------------------------------------------------------- */
#define ROWS           4
#define SYMBOLS        3
#define DIGITS         81        /* 3^4  possible column values */
#define MAX_NODES      6561      /* DIGITS * DIGITS */

/* --------------------------------------------------------------------- */
/* Pre‑computed digits of each possible column value                    */
/* --------------------------------------------------------------------- */
static uint8_t colDigits[MAX_NODES][ROWS];

/* --------------------------------------------------------------------- */
/* For each node we store its left and right column indices              */
/* and the determinants (shifted by +4 to be in 0..8)                     */
/* --------------------------------------------------------------------- */
static uint16_t nodeStart[MAX_NODES];          /* left column index     */
static uint16_t nodeEnd  [MAX_NODES];          /* right column index    */
static uint8_t  nodeDet  [MAX_NODES][3];       /* det + 4 for each block */

/* --------------------------------------------------------------------- */
/* For quick lookup of all nodes that start with a given left column     */
/* --------------------------------------------------------------------- */
static uint16_t startGroup[DIGITS][MAX_NODES];
static uint16_t startCount[DIGITS];

/* --------------------------------------------------------------------- */
/* Adjacency lists: for every node a dynamic array of successors        */
/* --------------------------------------------------------------------- */
static uint16_t **adjList = NULL;  /* array of pointers to uint16_t   */
static uint16_t *adjSize = NULL;   /* number of edges per node        */
static uint16_t *adjCap  = NULL;   /* capacity of each edge array     */
static uint16_t nodeCount = 0;     /* total number of nodes           */

/* --------------------------------------------------------------------- */
/* DP buffers for current and next length of paths                       */
/* --------------------------------------------------------------------- */
static seq_t *curBuf = NULL;
static seq_t *nextBuf = NULL;

/* --------------------------------------------------------------------- */
/* Cache of already computed a(n) values                                 */
/* --------------------------------------------------------------------- */
static seq_t *cache   = NULL;
static size_t cacheCap = 0;
static size_t cacheSize = 0;      /* number of stored terms (0 unused) */

/* --------------------------------------------------------------------- */
/* State flag – true after the graph has been built                      */
/* --------------------------------------------------------------------- */
static bool graphBuilt = false;

/* --------------------------------------------------------------------- */
/* Utility: add an edge from node u to node v (dynamic array handling)    */
/* --------------------------------------------------------------------- */
static void add_edge(uint16_t u, uint16_t v)
{
    if (adjSize[u] == adjCap[u]) {
        size_t newCap = adjCap[u] ? adjCap[u] * 2 : 4;
        uint16_t *newPtr = (uint16_t *)realloc(adjList[u], newCap * sizeof(uint16_t));
        if (!newPtr) exit(1);
        adjList[u] = newPtr;
        adjCap[u] = (uint16_t)newCap;
    }
    adjList[u][adjSize[u]++] = v;
}

/* --------------------------------------------------------------------- */
/* Build the directed graph of valid column pairs and their adjacency    */
/* --------------------------------------------------------------------- */
static void build_graph(void)
{
    /* 1. compute decimal digits of each column value (base‑3) */
    for (int idx = 0; idx < DIGITS; ++idx) {
        int v = idx;
        for (int r = ROWS - 1; r >= 0; --r) {
            colDigits[idx][r] = (uint8_t)(v % 3);
            v /= 3;
        }
    }

    /* 2. enumerate all pairs of columns, keep only those that satisfy
       the permanent inequality rule (vertical direction).            */
    nodeCount = 0;
    for (int c0 = 0; c0 < DIGITS; ++c0) {
        for (int c1 = 0; c1 < DIGITS; ++c1) {
            uint8_t perm[3], detAdj[3];
            bool ok = true;
            for (int r = 0; r < 3; ++r) {
                int a = colDigits[c0][r];
                int b = colDigits[c1][r];
                int c = colDigits[c0][r + 1];
                int d = colDigits[c1][r + 1];
                int ad = a * d;
                int bc = b * c;
                perm[r] = (uint8_t)(ad + bc);          /* permanent 0..8 */
                detAdj[r] = (uint8_t)(ad - bc + 4);    /* determinant+4 */
            }
            if (perm[0] == perm[1] || perm[1] == perm[2]) continue; /* fail */

            nodeStart[nodeCount] = (uint16_t)c0;
            nodeEnd  [nodeCount] = (uint16_t)c1;
            for (int r = 0; r < 3; ++r) nodeDet[nodeCount][r] = detAdj[r];

            startGroup[c0][startCount[c0]++] = (uint16_t)nodeCount;
            ++nodeCount;
        }
    }

    /* 3. allocate adjacency array structures (with explicit casts)      */
    adjList = (uint16_t **)calloc(nodeCount, sizeof(uint16_t *));
    adjSize = (uint16_t *)calloc(nodeCount, sizeof(uint16_t));
    adjCap  = (uint16_t *)calloc(nodeCount, sizeof(uint16_t));
    if (!adjList || !adjSize || !adjCap) exit(1);

    /* 4. for each node, add directed edges to all nodes that can
       follow it according to the determinant rule.                      */
    for (uint16_t u = 0; u < nodeCount; ++u) {
        uint16_t endCol = nodeEnd[u];
        uint16_t candCount = startCount[endCol];
        uint16_t *candidates = startGroup[endCol];
        for (uint16_t k = 0; k < candCount; ++k) {
            uint16_t v = candidates[k];
            bool diff = true;
            for (int r = 0; r < 3; ++r) {
                if (nodeDet[u][r] == nodeDet[v][r]) { diff = false; break; }
            }
            if (diff) add_edge(u, v);
        }
    }

    /* 5. allocate DP buffers for path counting */
    curBuf  = (seq_t *)calloc(nodeCount, sizeof(seq_t));
    nextBuf = (seq_t *)calloc(nodeCount, sizeof(seq_t));
    if (!curBuf || !nextBuf) exit(1);

    /* 6. initialise current counts for length 1 (single node arrays)   */
    for (uint16_t i = 0; i < nodeCount; ++i) curBuf[i] = (seq_t)1;

    /* 7. compute a(1) as total number of nodes */
    seq_t a1 = (seq_t)0;
    for (uint16_t i = 0; i < nodeCount; ++i) a1 += curBuf[i];

    /* 8. initialise cache with a(1) */
    cacheCap = 256;
    cache = (seq_t *)calloc(cacheCap, sizeof(seq_t));
    if (!cache) exit(1);
    cache[1] = a1;
    cacheSize = 2;   /* index 0 unused, 1 stored */

    graphBuilt = true;
}

/* --------------------------------------------------------------------- */
/* Sequence accessor function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Build graph on first call */
    if (!graphBuilt) build_graph();

    /* Extend cache until it contains a(n) */
    while (cacheSize <= n) {
        /* zero next buffer */
        for (uint16_t i = 0; i < nodeCount; ++i) nextBuf[i] = (seq_t)0;

        /* propagate path counts along directed edges */
        for (uint16_t u = 0; u < nodeCount; ++u) {
            if (curBuf[u] == (seq_t)0) continue;
            uint16_t outCnt = adjSize[u];
            for (uint16_t k = 0; k < outCnt; ++k) {
                uint16_t v = adjList[u][k];
                nextBuf[v] += curBuf[u];
            }
        }

        /* sum of all path counts gives a(next length) */
        seq_t total = (seq_t)0;
        for (uint16_t i = 0; i < nodeCount; ++i) total += nextBuf[i];

        /* grow cache if needed */
        if (cacheSize >= cacheCap) {
            size_t newCap = cacheCap * 2;
            seq_t *newCache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
            if (!newCache) exit(1);
            /* initialise new entries to zero */
            for (size_t i = cacheCap; i < newCap; ++i) newCache[i] = (seq_t)0;
            cache = newCache;
            cacheCap = newCap;
        }
        cache[cacheSize] = total;
        ++cacheSize;

        /* prepare for next iteration */
        seq_t *tmp = curBuf;
        curBuf  = nextBuf;
        nextBuf = tmp;
    }

    return cache[n];
}
