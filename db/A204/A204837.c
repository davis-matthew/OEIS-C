/*
 * A204837  Number of (n+1)X6 0..2 arrays with the permanents of all 2X2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts the number of (n+1)×6 arrays with entries 0..2
   for which every 2×2 sub‑block has the same non‑zero permanent.
   The proof reduces the problem to counting directed walks of length n
   in eight sparse directed graphs G_k (k = 1..8).  An edge from row r
   to row s exists iff for all column indices c = 0..4
        r[c]*s[c+1] + r[c+1]*s[c] = k   (k > 0).
   The number of such arrays is the sum over k of the total number of
   walks of length n in G_k, where a walk may start at any row.
   The graph has 3^6 = 729 vertices and at most 2048 directed edges in
   total; thus the dynamic programming has O(n·E) complexity (E ≤ 2048). */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#define NUM_ROWS 729          /* 3^6 */

seq_t a(idx_t n) {
    /* Static data structures for the six‑column state graphs. */

    static bool initialized = false;
    static int *edges[9];                 /* edges[k][i] -> destination vertex */
    static size_t startIdx[9][NUM_ROWS+1];/* adjacency offsets for each source */
    static size_t edgeCnt[9];

    /* ------------------------------------------------------------------
       Build the graph data once on the first call.
       ------------------------------------------------------------------ */
    if (!initialized) {
        /* Pre‑compute the base‑3 digits of all 729 rows (least significant
           digit is column 0). */
        static int digit[NUM_ROWS][6];
        for (size_t id = 0; id < NUM_ROWS; ++id) {
            int tmp = (int)id;
            for (int pos = 0; pos < 6; ++pos) {
                digit[id][pos] = tmp % 3;
                tmp /= 3;
            }
        }

        /* Count outgoing edges per vertex for each k. */
        static size_t outCnt[9][NUM_ROWS];
        for (int i = 0; i < 9; ++i)
            for (size_t v = 0; v < NUM_ROWS; ++v)
                outCnt[i][v] = 0;
        for (size_t src = 0; src < NUM_ROWS; ++src) {
            for (size_t dst = 0; dst < NUM_ROWS; ++dst) {
                int p0 = digit[src][0] * digit[dst][1] + digit[src][1] * digit[dst][0];
                if (p0 == 0) continue;
                bool ok = true;
                for (int c = 1; c < 5; ++c) {
                    int p = digit[src][c] * digit[dst][c+1] + digit[src][c+1] * digit[dst][c];
                    if (p != p0) { ok = false; break; }
                }
                if (!ok) continue;
                ++edgeCnt[p0];
                outCnt[p0][src] += 1;
            }
        }

        /* Compute adjacency offsets and allocate edge arrays. */
        for (int k = 1; k <= 8; ++k) {
            startIdx[k][0] = 0;
            for (size_t v = 0; v < NUM_ROWS; ++v)
                startIdx[k][v+1] = startIdx[k][v] + outCnt[k][v];
            edges[k] = (int*)malloc(edgeCnt[k] * sizeof(int));
        }

        /* Second pass: fill the edge lists. */
        static size_t curOut[9][NUM_ROWS];
        for (int i = 0; i < 9; ++i)
            for (size_t v = 0; v < NUM_ROWS; ++v)
                curOut[i][v] = 0;

        for (size_t src = 0; src < NUM_ROWS; ++src) {
            for (size_t dst = 0; dst < NUM_ROWS; ++dst) {
                int p0 = digit[src][0] * digit[dst][1] + digit[src][1] * digit[dst][0];
                if (p0 == 0) continue;
                bool ok = true;
                for (int c = 1; c < 5; ++c) {
                    int p = digit[src][c] * digit[dst][c+1] + digit[src][c+1] * digit[dst][c];
                    if (p != p0) { ok = false; break; }
                }
                if (!ok) continue;
                size_t idx = startIdx[p0][src] + curOut[p0][src];
                edges[p0][idx] = (int)dst;
                curOut[p0][src] += 1;
            }
        }
        initialized = true;
    }

    /* ------------------------------------------------------------------
       Count directed walks of length n for each k and sum them.
       ------------------------------------------------------------------ */
    seq_t result = 0;

    for (int k = 1; k <= 8; ++k) {
        /* DP arrays */
        seq_t dpCurr[NUM_ROWS];
        seq_t dpNext[NUM_ROWS];
        seq_t *curr = dpCurr;
        seq_t *next = dpNext;

        /* All start vertices are allowed: 1 path of length 0 ending at each. */
        for (size_t i = 0; i < NUM_ROWS; ++i) curr[i] = (seq_t)1;

        /* Walk for n steps. */
        for (idx_t step = 1; step <= n; ++step) {
            for (size_t i = 0; i < NUM_ROWS; ++i) next[i] = (seq_t)0;
            for (size_t src = 0; src < NUM_ROWS; ++src) {
                seq_t val = curr[src];
                if (val == 0) continue;
                size_t s = startIdx[k][src];
                size_t e = startIdx[k][src+1];
                for (size_t t = s; t < e; ++t) {
                    size_t dst = (size_t)edges[k][t];
                    next[dst] = next[dst] + val;
                }
            }
            /* swap current / next */
            seq_t *tmp = curr; curr = next; next = tmp;
        }

        /* Sum over all end vertices: total number of walks of length n. */
        seq_t sum = 0;
        for (size_t i = 0; i < NUM_ROWS; ++i)
            sum = sum + curr[i];
        result = result + sum;
    }
    return result;
}
