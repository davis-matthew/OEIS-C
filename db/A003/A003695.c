/*
 * A003695  Number of Hamiltonian paths in P_4 X P_n.
 * https://oeis.org/A003695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   transfer-matrix (plug/frontier) dynamic programming over the 4 x
 *            n grid, processed cell by cell; a(n) = number of edge sets
 *            forming a single path through all 4n cells
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   gp, grow_zero, sp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"
#include "../utils/gp.h"
#include "../utils/sp.h"

#define WW 4
#define NST 4096
#define DONE (2 << 10)

static seq_t st_cur[NST], st_nxt[NST];
static int on_cur[NST], on_nxt[NST];
static int lst_cur[NST], lst_nxt[NST];
static int n_cur, n_nxt;

static int partner(int s, int i) {
    int v = gp(s, i), d = 0, j;
    if (v == 1) {
        for (j = i; j <= WW; j++) {
            int u = gp(s, j);
            if (u == 1) d++;
            else if (u == 2) { d--; if (d == 0) return j; }
        }
    } else {
        for (j = i; j >= 0; j--) {
            int u = gp(s, j);
            if (u == 2) d++;
            else if (u == 1) { d--; if (d == 0) return j; }
        }
    }
    return -1;
}

static void emit(int t, seq_t v, int lastcol) {
    if (t == DONE && !lastcol) return;
    if (!on_nxt[t]) {
        on_nxt[t] = 1;
        lst_nxt[n_nxt++] = t;
        st_nxt[t] = v;
    } else {
        st_nxt[t] += v;
    }
}

static void swap_lists(void) {
    int i;
    for (i = 0; i < n_cur; i++) on_cur[lst_cur[i]] = 0;
    n_cur = 0;
    for (i = 0; i < n_nxt; i++) {
        int s = lst_nxt[i];
        on_nxt[s] = 0;
        on_cur[s] = 1;
        st_cur[s] = st_nxt[s];
        lst_cur[n_cur++] = s;
    }
    n_nxt = 0;
}

static void compute(seq_t *res, idx_t N) {
    int i;
    idx_t row;
    for (i = 0; i < NST; i++) { on_cur[i] = 0; on_nxt[i] = 0; }
    n_cur = 0; n_nxt = 0;
    on_cur[0] = 1; st_cur[0] = 1; lst_cur[n_cur++] = 0;
    for (row = 1; row <= N; row++) {
        int c;
        for (c = 0; c < WW; c++) {
            int canR = (c < WW - 1);
            int lastcol = (c == WW - 1);
            for (i = 0; i < n_cur; i++) {
                int s = lst_cur[i];
                seq_t val = st_cur[s];
                int x = gp(s, c), y = gp(s, c + 1);
                int e = s >> 10;
                int base = sp(sp(s, c, 0), c + 1, 0);
                if (s == DONE) continue;
                if (x == 0 && y == 0) {
                    if (canR) emit(sp(sp(base, c, 1), c + 1, 2), val, lastcol);
                    if (e < 2) {
                        emit(sp(base, c, 3) + (1 << 10), val, lastcol);
                        if (canR) emit(sp(base, c + 1, 3) + (1 << 10), val, lastcol);
                    }
                } else if (x == 0 || y == 0) {
                    int v = x + y;
                    int pos = x ? c : c + 1;
                    emit(sp(base, c, v), val, lastcol);
                    if (canR) emit(sp(base, c + 1, v), val, lastcol);
                    if (e < 2) {
                        if (v == 3) {
                            int t = base + (1 << 10);
                            if ((t & 1023) == 0) emit(t, val, lastcol);
                        } else {
                            int p = partner(s, pos);
                            emit(sp(base, p, 3) + (1 << 10), val, lastcol);
                        }
                    }
                } else {
                    if (x == 1 && y == 2) continue;
                    if (x == 3 && y == 3) {
                        if ((base & 1023) == 0) emit(base, val, lastcol);
                    } else if (x == 3) {
                        emit(sp(base, partner(s, c + 1), 3), val, lastcol);
                    } else if (y == 3) {
                        emit(sp(base, partner(s, c), 3), val, lastcol);
                    } else if (x == 1 && y == 1) {
                        emit(sp(base, partner(s, c + 1), 1), val, lastcol);
                    } else if (x == 2 && y == 2) {
                        emit(sp(base, partner(s, c), 2), val, lastcol);
                    } else {
                        emit(base, val, lastcol);
                    }
                }
            }
            swap_lists();
        }
        res[row] = 0;
        if (on_cur[DONE]) res[row] = st_cur[DONE];
        for (i = 0; i < n_cur; i++) {
            int s = lst_cur[i];
            int ns;
            if (s == DONE) continue;
            ns = ((s & 255) << 2) | (s & 3072);
            emit(ns, st_cur[s], 1);
        }
        swap_lists();
    }
}

seq_t a(idx_t n) {
    static seq_t *res = NULL;
    static idx_t have = 0;
    if (n > have) {
        idx_t newN = n;
        if (newN < 2 * have) newN = 2 * have;
        res = grow_zero(res, have ? have + 1 : 0, newN + 1);
        compute(res, newN);
        have = newN;
    }
    return res[n];
}
