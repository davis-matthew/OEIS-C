/*
 * A223502  Petersen graph (3,1) coloring a rectangular array: number of nX6
 *          0..5 arrays where 0..5 label nodes of a graph with edges 0,1 0,3
 *          3,5 3,4 1,2 1,4 4,5 2,0 2,5 and every array movement to a
 *          horizontal, diagonal or antidiagonal neighbor moves along an edge
 *          of this graph, with the array starting at 0.
 * https://oeis.org/A223502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 arrays of colors {0,…,5} such that
 *            horizontal, diagonal, and antidiagonal moves follow the edges of
 *            the Petersen (3,1) graph. Sequence A223502
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;              /* not used but safe */

    /* ---------- static memoisation structures ---------- */
    static bool initialized = false;
    static uint8_t (*rows)[6] = NULL;      /* array of valid row patterns */
    static size_t S = 0;                   /* number of states    */
    static size_t *adjCount = NULL;        /* outgoing degree per state   */
    static uint16_t **adjList = NULL;      /* transition lists          */
    static seq_t *cur = NULL, *next = NULL;/* DP vectors                  */
    static seq_t *res = NULL;              /* cached results          */
    static size_t resCap = 0;              /* capacity of res         */
    static idx_t computed = 0;             /* highest n computed      */

    if (!initialized) {
        /* -------- adjacency matrix of the Petersen (3,1) graph -------- */
        static unsigned char adj[6][6] = {0};
        int edges[9][2] = {
            {0,1},{0,3},{3,5},{3,4},
            {1,2},{1,4},{4,5},
            {2,0},{2,5}
        };
        for (int i = 0; i < 9; ++i) {
            int u = edges[i][0], v = edges[i][1];
            adj[u][v] = adj[v][u] = 1;
        }

        /* -------- enumerate all 6‑length color strings satisfying horizontal adjacency -------- */
        const size_t MAXPAT = 46656;  /* 6^6 */
        rows = (uint8_t (*)[6])malloc(MAXPAT * sizeof(*rows));
        size_t count = 0;
        for (int a0 = 0; a0 < 6; ++a0)
        for (int a1 = 0; a1 < 6; ++a1)
        for (int a2 = 0; a2 < 6; ++a2)
        for (int a3 = 0; a3 < 6; ++a3)
        for (int a4 = 0; a4 < 6; ++a4)
        for (int a5 = 0; a5 < 6; ++a5) {
            uint8_t pat[6] = { (uint8_t)a0,(uint8_t)a1,(uint8_t)a2,
                               (uint8_t)a3,(uint8_t)a4,(uint8_t)a5 };
            bool ok = true;
            for (int k = 0; k < 5; ++k) {
                if (!adj[pat[k]][pat[k+1]]) { ok = false; break; }
            }
            if (ok) {
                memcpy(rows[count], pat, 6);
                ++count;
            }
        }
        S = count;

        /* -------- build transition lists between rows -------- */
        adjCount = (size_t *)malloc(S * sizeof(*adjCount));

        /* first pass: count outgoing edges */
        for (size_t i = 0; i < S; ++i) {
            size_t c = 0;
            for (size_t j = 0; j < S; ++j) {
                bool ok = true;
                for (int col = 0; col < 6; ++col) {
                    uint8_t curc = rows[i][col];
                    if (col > 0 && !adj[curc][rows[j][col-1]]) { ok = false; break; }
                    if (col < 5 && !adj[curc][rows[j][col+1]]) { ok = false; break; }
                }
                if (ok) ++c;
            }
            adjCount[i] = c;
        }

        /* second pass: allocate and fill adjacency lists */
        adjList = (uint16_t **)malloc(S * sizeof(*adjList));
        for (size_t i = 0; i < S; ++i) {
            adjList[i] = (uint16_t *)malloc(adjCount[i] * sizeof(uint16_t));
            size_t pos = 0;
            for (size_t j = 0; j < S; ++j) {
                bool ok = true;
                for (int col = 0; col < 6; ++col) {
                    uint8_t curc = rows[i][col];
                    if (col > 0 && !adj[curc][rows[j][col-1]]) { ok = false; break; }
                    if (col < 5 && !adj[curc][rows[j][col+1]]) { ok = false; break; }
                }
                if (ok) adjList[i][pos++] = (uint16_t)j;
            }
        }

        /* -------- initialise DP vectors ---------- */
        cur  = (seq_t *)malloc(S * sizeof(*cur));
        next = (seq_t *)malloc(S * sizeof(*next));
        for (size_t i = 0; i < S; ++i)
            cur[i] = (rows[i][0] == 0) ? (seq_t)1 : (seq_t)0;

        /* -------- initial result (n = 1) ---------- */
        res = (seq_t *)malloc(2 * sizeof(*res));
        res[0] = (seq_t)0;                     /* unused for index 0 */
        seq_t total = 0;
        for (size_t i = 0; i < S; ++i) total += cur[i];
        res[1] = total;
        resCap     = 2;
        computed   = 1;
        initialized = true;
    }

    /* -------- compute up to requested n -------- */
    if (n > computed) {
        for (idx_t lev = computed + 1; lev <= n; ++lev) {
            /* zero next vector */
            memset(next, 0, S * sizeof(*next));

            /* one transition step */
            for (size_t i = 0; i < S; ++i) {
                seq_t cnt = cur[i];
                if (!cnt) continue;
                uint16_t *lst = adjList[i];
                size_t deg = adjCount[i];
                for (size_t k = 0; k < deg; ++k) {
                    next[lst[k]] += cnt;
                }
            }

            /* swap cur and next */
            seq_t *tmp = cur; cur = next; next = tmp;

            /* accumulate result for this level */
            seq_t sum = 0;
            for (size_t i = 0; i < S; ++i) sum += cur[i];

            /* ensure capacity of res */
            if ((size_t)(computed + 1) >= resCap) {
                resCap = resCap * 2;
                res = (seq_t *)realloc(res, resCap * sizeof(*res));
            }
            res[computed + 1] = sum;
            ++computed;
        }
    }

    return res[(size_t)n];
}
