/*
 * A206506  Number of (n+1)X3 0..2 arrays with the number of clockwise edge
 *          increases in 2X2 subblocks nondecreasing rightwards and downwards.
 * https://oeis.org/A206506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming on rows (n+1)x3 arrays)
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   edge_inc, get_row_vals, subblock_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef A206506_H
#define A206506_H

#include <stddef.h>
#include <stdint.h>
#include "../utils/edge_inc.h"
#include "../utils/get_row_vals.h"
#include "../utils/subblock_counts.h"

/* Main function returning the nth term.  n >= 1.  */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    /* Precompute tables for all 27 possible rows. */
    enum { ROWS = 27, STATES = ROWS * ROWS };
    static uint8_t L[STATES];
    static uint8_t R[STATES];
    static uint8_t valid[STATES];
    static uint8_t deg[STATES];
    static uint8_t adj[STATES][ROWS]; /* list of next rows */
    static int  initialized = 0;

    if (!initialized) {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < ROWS; ++j) {
                int idx = i * ROWS + j;
                uint8_t l, r;
                subblock_counts(i, j, &l, &r);
                L[idx] = l;
                R[idx] = r;
                valid[idx] = (l <= r) ? 1u : 0u;
            }
        }
        /* Build adjacency lists. */
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < ROWS; ++j) {
                int p = i * ROWS + j;
                deg[p] = 0;
                if (!valid[p]) continue;
                uint8_t l_prev = L[p];
                uint8_t r_prev = R[p];
                for (int k = 0; k < ROWS; ++k) {
                    int qidx = j * ROWS + k;
                    if (!valid[qidx]) continue;
                    uint8_t l_next = L[qidx];
                    uint8_t r_next = R[qidx];
                    if (l_prev <= l_next && r_prev <= r_next) {
                        adj[p][deg[p]++] = (uint8_t)k;
                    }
                }
            }
        }
        initialized = 1;
    }

    /* Dynamic programming arrays. */
    static seq_t cur[STATES];
    static seq_t nxt[STATES];
    /* Initialize for sequences of two rows. */
    for (int i = 0; i < STATES; ++i) cur[i] = 0;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            int idx = i * ROWS + j;
            if (valid[idx]) cur[idx] = 1;
        }
    }

    /* If only one pair of rows (n=1), sum cur. */
    if (n == (idx_t)1) {
        seq_t sum = 0;
        for (int i = 0; i < STATES; ++i) sum += cur[i];
        return sum;
    }

    /* Add rows one by one. */
    for (idx_t step = 2; step <= n; ++step) {
        for (int i = 0; i < STATES; ++i) nxt[i] = 0;
        for (int p = 0; p < STATES; ++p) {
            seq_t val = cur[p];
            if (val == 0) continue;
            int j = p % ROWS;
            int d = deg[p];
            for (int t = 0; t < d; ++t) {
                uint8_t k = adj[p][t];
                int next_id = j * ROWS + k;
                nxt[next_id] += val;
            }
        }
        /* swap cur and nxt */
        for (int i = 0; i < STATES; ++i) cur[i] = nxt[i];
    }

    /* Sum all states for final answer. */
    seq_t ans = 0;
    for (int i = 0; i < STATES; ++i) ans += cur[i];
    return ans;
}
#endif /* A206506_H */
