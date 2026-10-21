/*
 * A223411  4X4X4 triangular graph without horizontal edges coloring a
 *          rectangular array: number of nX4 0..9 arrays where 0..9 label
 *          nodes of a graph with edges 0,1 0,2 1,3 1,4 2,4 2,5 3,6 3,7 4,7
 *          4,8 5,8 5,9 and every array movement to a horizontal or vertical
 *          neighbor moves along an edge of this graph.
 * https://oeis.org/A223411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 1ᵀ · Mⁿ⁻¹ · 1   (n ≥ 1)
 *
 * Explanation:
 *   M is the compatibility matrix of the 4‑digit row patterns that
 *   satisfy the horizontal edges of the graph.  Entry M[i][j] = 1
 *   iff pattern i can be placed above pattern j (all four vertical
 *   neighbour pairs are edges).  a(1) equals the number of admissible
 *   row patterns.  Increasing the height by one multiplies the
 *   current column vector of counts by M.  Thus a(n) is the sum of
 *   the column vector after n‑1 such multiplications.
 *
 * The code builds the patterns and M once, then uses a simple
 * dynamic‑programming recurrence to extend the computation for
 * arbitrary n.  Results are cached so repeated calls are O(1).
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define DIGIT_MAX 10          /* digits 0–9 */
#define MAX_K 200             /* maximum number of row patterns */

/* --------------------------------------------------------------------- */
/* Global state (initialised on first call)                              */
/* --------------------------------------------------------------------- */
static bool        _initialized = false;
static size_t      _K = 0;                     /* number of patterns */

/* adjacency of the graph: 1 = edge present */
static uint8_t     _edge[DIGIT_MAX][DIGIT_MAX] = {0};

/* all admissible 4‑digit row patterns */
struct pattern { uint8_t d[4]; };
static struct pattern _patterns[MAX_K];

/* compatibility matrix: _compat[i][j] == 1 iff pattern i can be above j */
static uint8_t  _compat[MAX_K][MAX_K] = {0};

/* DP buffers: at most MAX_K entries each */
static seq_t    _dp_prev[MAX_K];
static seq_t    _dp_next[MAX_K];

/* Cache of previously computed a(n) (1‑based indexing) */
static seq_t    _cache[10001] = {0};
static idx_t    _cached_n = 0;        /* last n for which _cache contains a(n) */

/* --------------------------------------------------------------------- */
/* Build function: sets up the graph, generates patterns, builds
 * compatibility matrix, and computes a(1). */
static void _build(void)
{
    /* 1. Build adjacency matrix */
    const int edge_list[][2] = {
        {0,1},{0,2},{1,3},{1,4},{2,4},{2,5},
        {3,6},{3,7},{4,7},{4,8},{5,8},{5,9}
    };
    for (int k = 0; k < 12; ++k) {
        int u = edge_list[k][0];
        int v = edge_list[k][1];
        _edge[u][v] = 1;
        _edge[v][u] = 1;
    }

    /* 2. Enumerate all admissible 4‑digit row patterns */
    for (int a = 0; a < DIGIT_MAX; ++a) {
        for (int b = 0; b < DIGIT_MAX; ++b) {
            if (!_edge[a][b]) continue;
            for (int c = 0; c < DIGIT_MAX; ++c) {
                if (!_edge[b][c]) continue;
                for (int d = 0; d < DIGIT_MAX; ++d) {
                    if (!_edge[c][d]) continue;
                    _patterns[_K].d[0] = (uint8_t)a;
                    _patterns[_K].d[1] = (uint8_t)b;
                    _patterns[_K].d[2] = (uint8_t)c;
                    _patterns[_K].d[3] = (uint8_t)d;
                    ++_K;
                    if (_K >= MAX_K) return;   /* safety, should never happen */
                }
            }
        }
    }

    /* 3. Build compatibility matrix */
    for (size_t i = 0; i < _K; ++i) {
        for (size_t j = 0; j < _K; ++j) {
            bool ok = true;
            for (int p = 0; p < 4 && ok; ++p) {
                ok &= _edge[_patterns[i].d[p]][_patterns[j].d[p]];
            }
            _compat[i][j] = ok;
        }
    }

    /* 4. Initialise DP for n = 1 */
    seq_t total = 0;
    for (size_t i = 0; i < _K; ++i) {
        _dp_prev[i] = (seq_t)1;
        total += _dp_prev[i];
    }
    _cache[1] = total;
    _cached_n = 1;
}

/* --------------------------------------------------------------------- */
/* Public function: returns a(n) for n ≥ 1.  Undefined for n = 0.       */
seq_t a(idx_t n)
{
    if (!_initialized) {
        _build();
        _initialized = true;
    }

    if (n == 0) return 0;               /* undefined for offset 1 */

    if (n <= _cached_n) return _cache[(size_t)n];

    /* compute consecutively up to n */
    for (idx_t cur = _cached_n + 1; cur <= n; ++cur) {
        memset(_dp_next, 0, sizeof(_dp_next));

        for (size_t i = 0; i < _K; ++i) {
            seq_t val = _dp_prev[i];
            if (!val) continue;
            for (size_t j = 0; j < _K; ++j) {
                if (_compat[i][j]) {
                    _dp_next[j] += val;
                }
            }
        }

        seq_t sum = 0;
        for (size_t j = 0; j < _K; ++j) sum += _dp_next[j];

        _cache[(size_t)cur] = sum;
        memcpy(_dp_prev, _dp_next, sizeof(_dp_prev));
    }

    _cached_n = n;
    return _cache[(size_t)n];
}
