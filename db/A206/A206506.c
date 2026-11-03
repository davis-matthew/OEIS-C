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
 * Validated: standard, pass, 32 terms through n = 207 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef A206506_H
#define A206506_H

#include <stddef.h>
#include <stdint.h>

/* Helper functions to compute the number of clockwise edge increases
   in a 2x2 subblock.  The subblock is defined by two consecutive rows
   each of 3 columns with entries in {0,1,2}.  */
static inline uint8_t edge_inc(uint8_t a, uint8_t b)
{
    return (b > a) ? 1u : 0u;
}

static inline void get_row_vals(int idx, uint8_t *v0, uint8_t *v1, uint8_t *v2)
{
    *v0 = (uint8_t)(idx / 9);
    *v1 = (uint8_t)((idx / 3) % 3);
    *v2 = (uint8_t)(idx % 3);
}

static inline void subblock_counts(int row1, int row2, uint8_t *L, uint8_t *R)
{
    uint8_t a0, a1, a2, b0, b1, b2;
    get_row_vals(row1, &a0, &a1, &a2);
    get_row_vals(row2, &b0, &b1, &b2);
    /* left subblock (columns 0-1) */
    *L = edge_inc(a0, a1) + edge_inc(a1, b1) + edge_inc(b1, b0) + edge_inc(b0, a0);
    /* right subblock (columns 1-2) */
    *R = edge_inc(a1, a2) + edge_inc(a2, b2) + edge_inc(b2, b1) + edge_inc(b1, a1);
}

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
