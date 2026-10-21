/*
 * A205835  Number of (n+1)X8 0..2 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to two.
 * https://oeis.org/A205835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Number of (n+1)×8 arrays over {0,1,2} whose every 2×2 subblock has
 * exactly two clockwise edge increases.  Each row is treated as a state of length
 * 8 over {0,1,2}.  A directed transition from state P to Q is allowed iff for
 * every column j (0 ≤ j < 7) the block
 *   top-left   = P[j]
 *   top-right  = P[j+1]
 *   bottom-left= Q[j]
 *   bottom-right= Q[j+1]
 * has exactly two of the following comparisons strictly true:
 *   P[j] < P[j+1]  (right)
 *   P[j+1] < Q[j+1]  (down)
 *   Q[j+1] < Q[j]  (left)
 *   Q[j] < P[j]    (up)
 * The total number of arrays of height n+1 is the number of walks of length n
 * in this directed graph, which is computed by dynamic programming. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper constants */
#define STATE_COUNT 6561  /* 3^8 */

/* Global data used for adjacency construction */
static uint16_t *neighbors[STATE_COUNT];
static size_t   neigh_len[STATE_COUNT];
static size_t   neigh_cap[STATE_COUNT];
static uint8_t  top_row[8];
static const int pow3[8] = {2187, 729, 243, 81, 27, 9, 3, 1};

/* Helper to add a neighbor, growing storage as needed */
static void add_neighbor(idx_t src, idx_t trg) {
    if (neigh_len[src] == neigh_cap[src]) {
        neigh_cap[src] = neigh_cap[src] ? neigh_cap[src] * 2 : 4;
        neighbors[src] = (uint16_t *)realloc(neighbors[src], neigh_cap[src] * sizeof(uint16_t));
    }
    neighbors[src][neigh_len[src]++] = (uint16_t)trg;
}

/* Recursive construction of all valid successor rows for a given top row */
static void dfs_build(idx_t src, int pos, int prev, int code) {
    if (pos == 8) {
        add_neighbor(src, (idx_t)code);
        return;
    }
    if (pos == 0) {
        /* First element – no block to check yet */
        for (int v = 0; v < 3; ++v) {
            dfs_build(src, 1, v, v);
        }
    } else {
        /* Check block formed with columns pos-1 and pos */
        int a = top_row[pos - 1];
        int b = top_row[pos];
        int c = prev;        /* Q[pos-1] */
        for (int v = 0; v < 3; ++v) {
            int d = v;        /* Q[pos]   */
            int inc = (a < b) + (b < d) + (d < c) + (c < a);
            if (inc == 2) {
                dfs_build(src, pos + 1, v, code * 3 + v);
            }
        }
    }
}

/* Ensure the adjacency information is built once */
static void ensure_adjacency(void) {
    static int built = 0;
    if (built) return;
    for (idx_t i = 0; i < STATE_COUNT; ++i) {
        neigh_len[i] = 0;
        neigh_cap[i] = 0;
        neighbors[i] = NULL;
    }
    for (idx_t s = 0; s < STATE_COUNT; ++s) {
        /* Decode state s into top_row[0..7] */
        idx_t tmp = s;
        for (int k = 7; k >= 0; --k) {
            top_row[k] = (uint8_t)(tmp % 3);
            tmp /= 3;
        }
        dfs_build(s, 0, 0, 0);          /* build successors of state s */
    }
    built = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* value not defined for n=0 */
    ensure_adjacency();

    /* DP arrays */
    static seq_t cur[STATE_COUNT];
    static seq_t nxt[STATE_COUNT];
    size_t i;
    for (i = 0; i < STATE_COUNT; ++i) cur[i] = (seq_t)1;

    for (idx_t step = 1; step <= n; ++step) {
        /* set nxt to zero */
        for (i = 0; i < STATE_COUNT; ++i) nxt[i] = (seq_t)0;

        /* propagate counts */
        for (i = 0; i < STATE_COUNT; ++i) {
            seq_t val = cur[i];
            if (val == 0) continue;
            size_t len = neigh_len[i];
            uint16_t *vec = neighbors[i];
            for (size_t k = 0; k < len; ++k) {
                nxt[vec[k]] += val;
            }
        }
        /* copy nxt back to cur for next iteration */
        for (i = 0; i < STATE_COUNT; ++i) cur[i] = nxt[i];
    }

    /* sum all states to obtain the final count */
    seq_t result = (seq_t)0;
    for (i = 0; i < STATE_COUNT; ++i) result += cur[i];
    return result;
}
