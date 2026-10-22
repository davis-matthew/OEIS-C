/*
 * A003305  Figure 8's with 2n edges on the square lattice.
 * https://oeis.org/A003305
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, more
 * Formula:   exhaustive search. a(n) = 2*F(n), where F(n) counts figure 8's
 *            touching at the origin with loop A using the N and E edges there
 *            and loop B using the S and W edges. Each loop is traced by
 *            depth-first search as a self-avoiding walk with a fixed
 *            direction, and walks are pruned by their distance from...
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   f8_abs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/f8_abs.h"

static int f8_W, f8_off, f8_tot;
static unsigned char *f8_vis;
static uint64_t f8_cnt;

static void f8_B(int x, int y, int s, int M) {
    static const int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
    int d;
    if (x == -1 && y == 0) {
        if (s + 1 == M) f8_cnt++;
        return;
    }
    for (d = 0; d < 4; d++) {
        int nx = x + dx[d], ny = y + dy[d];
        int idx = (ny + f8_off) * f8_W + (nx + f8_off);
        if (f8_vis[idx]) continue;
        if (s + 1 + f8_abs(nx + 1) + f8_abs(ny) + 1 > M) continue;
        f8_vis[idx] = 1;
        f8_B(nx, ny, s + 1, M);
        f8_vis[idx] = 0;
    }
}

static void f8_A(int x, int y, int s) {
    static const int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
    int d;
    if (x == 1 && y == 0) {
        int L = s + 1, M = f8_tot - L;
        int i1 = (-1 + f8_off) * f8_W + (0 + f8_off);
        int i2 = (0 + f8_off) * f8_W + (-1 + f8_off);
        if (L >= 4 && M >= 4 && !f8_vis[i1] && !f8_vis[i2]) {
            f8_vis[i1] = 1;
            f8_B(0, -1, 1, M);
            f8_vis[i1] = 0;
        }
        return;
    }
    for (d = 0; d < 4; d++) {
        int nx = x + dx[d], ny = y + dy[d];
        int idx = (ny + f8_off) * f8_W + (nx + f8_off);
        if (f8_vis[idx]) continue;
        if (s + 1 + f8_abs(nx - 1) + f8_abs(ny) + 1 > f8_tot - 4) continue;
        f8_vis[idx] = 1;
        f8_A(nx, ny, s + 1);
        f8_vis[idx] = 0;
    }
}

seq_t a(idx_t n) {
    static uint64_t memo[64];
    static unsigned char have[64];
    uint64_t res;
    if (n < 4) return 0;
    if (n < 64 && have[n]) {
        res = memo[n];
    } else {
        f8_tot = (int)(2 * n);
        f8_off = (int)n + 2;
        f8_W = 2 * f8_off + 1;
        f8_vis = (unsigned char *)calloc((size_t)f8_W * (size_t)f8_W, 1);
        f8_vis[f8_off * f8_W + f8_off] = 1;              /* origin */
        f8_vis[(1 + f8_off) * f8_W + f8_off] = 1;        /* (0,1): first step of A */
        f8_cnt = 0;
        f8_A(0, 1, 1);
        free(f8_vis);
        f8_vis = NULL;
        res = f8_cnt;
        if (n < 64) { memo[n] = res; have[n] = 1; }
    }
    return (seq_t)res * 2;
}
