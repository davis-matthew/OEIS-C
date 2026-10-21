/*
 * A234791  Number of (n+1)X(3+1) 0..3 arrays with each 2X2 subblock having
 *          the number of clockwise edge increases less than or equal to the
 *          number of counterclockwise edge increases.
 * https://oeis.org/A234791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{rows r0,…,rn} 1 where every adjacent pair of rows is
 *            compatible: for each 2×2 sub‑block the number of clockwise edge
 *            increases ≤ the number of counter‑clockwise edge increases
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   init_graph
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/init_graph.h"

seq_t a(idx_t n)
{
    static bool graph_inited = false;
    static uint16_t NEIGH[256][256];
    static uint16_t NEIGH_CNT[256];
    static unsigned char ROWS[256][4];

    if (!graph_inited) {
        init_graph(NEIGH, NEIGH_CNT, ROWS);
        graph_inited = true;
    }

    /* dynamic programming over rows */
    seq_t cur[256];
    seq_t nxt[256];

    /* start with one row of any type */
    for (int i = 0; i < 256; ++i) cur[i] = 1;

    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 256; ++i) nxt[i] = 0;
        for (int i = 0; i < 256; ++i) {
            seq_t val = cur[i];
            if (val == 0) continue;
            uint16_t c = NEIGH_CNT[i];
            for (uint16_t k = 0; k < c; ++k) {
                uint16_t j = NEIGH[i][k];
                nxt[j] += val;
            }
        }
        for (int i = 0; i < 256; ++i) cur[i] = nxt[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 256; ++i) total += cur[i];
    return total;
}
