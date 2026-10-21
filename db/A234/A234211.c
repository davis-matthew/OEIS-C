/*
 * A234211  Number of (n+1)X(2+1) 0..4 arrays with every 2X2 subblock having
 *          the sum of the squares of all six edge and diagonal differences
 *          equal to 11.
 * https://oeis.org/A234211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,…,4} where each 2×2
 *            subblock has the sum of the squares of its six edge and diagonal
 *            differences equal to 11. Computed by dynamic programming on a
 *            graph of 125 states
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define ROWS 125
static int neighbors[ROWS][ROWS];
static uint8_t deg[ROWS];
static int decoded[ROWS][3];
static int build_done = 0;

static void build_graph(void)
{
    if (build_done) return;
    for (int i = 0; i < ROWS; ++i) {
        int x = i / 25;
        int y = (i / 5) % 5;
        int z = i % 5;
        decoded[i][0] = x;
        decoded[i][1] = y;
        decoded[i][2] = z;
    }
    for (int i = 0; i < ROWS; ++i) deg[i] = 0;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            int *r1 = decoded[i];
            int *r2 = decoded[j];
            int a = r1[0], b = r1[1], c = r1[2];
            int d = r2[0], e = r2[1], f = r2[2];
            int diff, term;

            diff = a > b ? a - b : b - a; term = diff * diff;
            diff = a > d ? a - d : d - a; term += diff * diff;
            diff = b > e ? b - e : e - b; term += diff * diff;
            diff = d > e ? d - e : e - d; term += diff * diff;
            diff = a > e ? a - e : e - a; term += diff * diff;
            diff = b > d ? b - d : d - b; term += diff * diff;
            if (term != 11) continue;

            term = 0;
            diff = b > c ? b - c : c - b; term += diff * diff;
            diff = b > e ? b - e : e - b; term += diff * diff;
            diff = c > f ? c - f : f - c; term += diff * diff;
            diff = e > f ? e - f : f - e; term += diff * diff;
            diff = b > f ? b - f : f - b; term += diff * diff;
            diff = c > e ? c - e : e - c; term += diff * diff;
            if (term != 11) continue;

            neighbors[i][deg[i]++] = j;
        }
    }
    build_done = 1;
}

seq_t a(idx_t n)
{
    build_graph();
    seq_t dpPrev[ROWS], dpNext[ROWS];
    for (int i = 0; i < ROWS; ++i) dpPrev[i] = 1;
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < ROWS; ++i) dpNext[i] = 0;
        for (int i = 0; i < ROWS; ++i) {
            seq_t val = dpPrev[i];
            if (val == 0) continue;
            for (int e = 0; e < deg[i]; ++e) {
                int j = neighbors[i][e];
                dpNext[j] += val;
            }
        }
        for (int i = 0; i < ROWS; ++i) dpPrev[i] = dpNext[i];
    }
    seq_t res = 0;
    for (int i = 0; i < ROWS; ++i) res += dpPrev[i];
    return res;
}
