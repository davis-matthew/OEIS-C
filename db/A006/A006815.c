/*
 * A006815  Related to self-avoiding walks on square lattice.
 * https://oeis.org/A006815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, more
 * Formula:   exhaustive search -- a(n) = sum over all n-step self-avoiding
 *            walks on the square lattice whose first step is fixed to the
 *            east, of the shortest-path distance from start to end once every
 *            pair of lattice-adjacent visited sites is joined by an edge.
 *            Mirror symmetry about the x-axis halves the sear...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static int g_n, g_W;
static int *g_d;          /* distance from start per cell, -1 = unoccupied */
static int *g_us_cell;    /* undo stack: cell */
static int *g_us_old;     /* undo stack: old distance */
static int g_sp;
static int *g_q;          /* propagation queue */

static void propagate(int c) {
    int head = 0, tail = 0;
    int off[4];
    off[0] = 1; off[1] = -1; off[2] = g_W; off[3] = -g_W;
    g_q[tail++] = c;
    while (head < tail) {
        int x = g_q[head++];
        int dx = g_d[x] + 1;
        for (int k = 0; k < 4; k++) {
            int y = x + off[k];
            if (g_d[y] > dx) {
                g_us_cell[g_sp] = y;
                g_us_old[g_sp] = g_d[y];
                g_sp++;
                g_d[y] = dx;
                g_q[tail++] = y;
            }
        }
    }
}

static seq_t dfs(int pos, int depth, bool turned) {
    if (depth == g_n) return (seq_t)g_d[pos];
    seq_t tot = 0;
    int off[4];
    off[0] = 1; off[1] = -1; off[2] = g_W; off[3] = -g_W;
    for (int k = 0; k < 4; k++) {
        if (!turned && k == 3) continue;   /* first turn is always "up" */
        int np = pos + off[k];
        if (g_d[np] >= 0) continue;
        int m = -1;
        for (int j = 0; j < 4; j++) {
            int v = g_d[np + off[j]];
            if (v >= 0 && (m < 0 || v < m)) m = v;
        }
        g_d[np] = m + 1;
        int sp = g_sp;
        propagate(np);
        bool nt = turned || k >= 2;
        seq_t r = dfs(np, depth + 1, nt);
        while (g_sp > sp) {
            g_sp--;
            g_d[g_us_cell[g_sp]] = g_us_old[g_sp];
        }
        g_d[np] = -1;
        if (!turned && nt) tot += r * 2;
        else tot += r;
    }
    return tot;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    int N = (int)n;
    g_n = N;
    g_W = 2 * N + 5;
    int cells = g_W * g_W;
    g_d = (int *)malloc((size_t)cells * sizeof(int));
    for (int i = 0; i < cells; i++) g_d[i] = -1;
    size_t ustack = (size_t)(N + 2) * (size_t)(N + 2) * (size_t)(N + 2) + 16;
    g_us_cell = (int *)malloc(ustack * sizeof(int));
    g_us_old = (int *)malloc(ustack * sizeof(int));
    g_q = (int *)malloc(((size_t)(N + 2) * (size_t)(N + 2) + 16) * sizeof(int));
    g_sp = 0;

    int origin = (N + 2) * g_W + (N + 2);
    g_d[origin] = 0;
    int first = origin + 1;
    g_d[first] = 1;

    seq_t res = dfs(first, 1, false);

    free(g_d);
    free(g_us_cell);
    free(g_us_old);
    free(g_q);
    return res;
}
