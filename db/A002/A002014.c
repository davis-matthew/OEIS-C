/*
 * A002014  Number of symmetric filaments (strip polyominoes) with n square
 *          cells.
 * https://oeis.org/A002014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   no closed formula; computed by a backtracking search. A filament
 *            is a path of n cells in which only consecutive cells touch. It
 *            is described by its sequence t of n-2 turns (S, L, R), and its
 *            shape up to symmetry is its orbit under {id, swap L<->R,
 *            reverse, reverse+swap}. A filament is symmetric i...
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static unsigned char *g_grid;
static int g_w;
static int g_m, g_h, g_type;
static int *g_t;
static seq_t g_cnt;

static const int DX[4] = {1, 0, -1, 0};
static const int DY[4] = {0, 1, 0, -1};

static bool try_add(int x, int y, int px, int py) {
    int d;
    if (g_grid[y * g_w + x]) return false;
    for (d = 0; d < 4; d++) {
        int nx = x + DX[d], ny = y + DY[d];
        if (nx == px && ny == py) continue;
        if (g_grid[ny * g_w + nx]) return false;
    }
    g_grid[y * g_w + x] = 1;
    return true;
}

static int sym_turn(int t) {
    if (g_type == 0) return t;
    return (4 - t) & 3;
}

static void complete(int k, int dir, int x, int y) {
    int xs[256], ys[256];
    int placed = 0, j, i;
    bool ok = true;
    for (j = k; j < g_m; j++) {
        int tv = sym_turn(g_t[g_m - 1 - j]);
        int nx, ny;
        g_t[j] = tv;
        dir = (dir + tv) & 3;
        nx = x + DX[dir];
        ny = y + DY[dir];
        if (!try_add(nx, ny, x, y)) { ok = false; break; }
        xs[placed] = nx; ys[placed] = ny; placed++;
        x = nx; y = ny;
    }
    if (ok) g_cnt++;
    for (i = 0; i < placed; i++) g_grid[ys[i] * g_w + xs[i]] = 0;
}

static void dfs(int k, int dir, int x, int y) {
    static const int choices[3] = {0, 1, 3};
    int c;
    if (k == g_h) {
        complete(k, dir, x, y);
        return;
    }
    for (c = 0; c < 3; c++) {
        int tv = choices[c];
        int nd, nx, ny;
        if (g_type == 1 && (g_m & 1) && k == (g_m - 1) / 2 && tv != 0) continue;
        nd = (dir + tv) & 3;
        nx = x + DX[nd];
        ny = y + DY[nd];
        if (!try_add(nx, ny, x, y)) continue;
        g_t[k] = tv;
        dfs(k + 1, nd, nx, ny);
        g_grid[ny * g_w + nx] = 0;
    }
}

static seq_t count_type(int n, int type) {
    int off = n + 2;
    g_type = type;
    g_m = n - 2;
    g_h = (g_m + 1) / 2;
    g_cnt = 0;
    memset(g_grid, 0, (size_t)g_w * (size_t)g_w);
    g_grid[off * g_w + off] = 1;
    g_grid[off * g_w + off + 1] = 1;
    dfs(0, 0, off + 1, off);
    return g_cnt;
}

seq_t a(idx_t n) {
    seq_t A, B, r;
    int nn;
    if (n <= 1) return 1;
    if (n > 250) return 0;
    nn = (int)n;
    g_w = 2 * nn + 5;
    g_grid = (unsigned char *)malloc((size_t)g_w * (size_t)g_w);
    g_t = (int *)malloc(sizeof(int) * (size_t)(nn + 2));
    A = count_type(nn, 0);
    B = count_type(nn, 1);
    free(g_grid);
    free(g_t);
    r = (A - 1) / 2 + (B - 1) / 2 + 1;
    return r;
}
