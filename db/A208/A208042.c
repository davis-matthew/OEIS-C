/*
 * A208042  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A208042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   horizontal_valid, vertical_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of 6×n binary arrays avoiding horizontal 000 and 010, and vertical 000 and 101.
 * This is implemented by a transfer‑matrix DP over allowed columns.
 * The state is a pair (previous‑previous column, previous column).
 * Transitions are added when the three consecutive columns satisfy the horizontal rule.
 * The algorithm runs in O(n·M²) time with M = 19 (number of allowed columns). */
#include <stdint.h>
#include "../utils/horizontal_valid.h"
#include "../utils/vertical_valid.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static data for the transfer matrix */
    static int M = 0;                         /* number of allowed columns           */
    static int total_states = 0;              /* M*M states                          */
    static uint8_t column[64];                /* allowed column bit patterns         */
    static int col_idx[64];                   /* mapping column value -> index       */
    static int edge_cnt[4096];                /* number of outgoing edges per state  */
    static int edges[4096][64];               /* next column indices for each state  */
    static int initialized = 0;

    /* DP storage */
    static seq_t cur[4096];
    static seq_t nxt[4096];

    if (!initialized) {
        /* Build list of allowed columns */
        for (int i = 0; i < 64; ++i) col_idx[i] = -1;
        M = 0;
        for (int c = 0; c < 64; ++c) {
            if (vertical_valid((uint8_t)c)) {
                col_idx[c] = M;
                column[M] = (uint8_t)c;
                ++M;
            }
        }
        total_states = M * M;

        /* Precompute transition edges for every state (prevprev, prev) */
        for (int s = 0; s < total_states; ++s) {
            int p2 = s / M;          /* index of previous‑previous column */
            int p1 = s % M;          /* index of previous column         */
            uint8_t c2 = column[p2];
            uint8_t c1 = column[p1];
            edge_cnt[s] = 0;
            for (int q = 0; q < M; ++q) {
                uint8_t c3 = column[q];
                if (horizontal_valid(c2, c1, c3)) {
                    edges[s][edge_cnt[s]++] = q;   /* next column index */
                }
            }
        }
        initialized = 1;
    }

    /* Handle small n directly */
    if (n == (idx_t)1)
        return (seq_t)M;
    if (n == (idx_t)2)
        return (seq_t)M * (seq_t)M;

    /* Initialise DP for n = 2 (every pair of allowed columns is valid) */
    for (int i = 0; i < total_states; ++i)
        cur[i] = 1;

    /* Transition from column 3 up to column n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < total_states; ++i)
            nxt[i] = 0;
        for (int s = 0; s < total_states; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            int pc = edge_cnt[s];
            for (int k = 0; k < pc; ++k) {
                int q = edges[s][k];
                int nid = (s % M) * M + q;   /* new state after adding column q */
                nxt[nid] += val;
            }
        }
        /* swap cur and nxt */
        for (int i = 0; i < total_states; ++i)
            cur[i] = nxt[i];
    }

    /* Sum all states to obtain the total number of arrays of width n */
    seq_t total = 0;
    for (int i = 0; i < total_states; ++i)
        total += cur[i];
    return total;
}
