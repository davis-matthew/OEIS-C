/*
 * A004481  Table of Sprague-Grundy values for Wythoff's game (Wyt Queens)
 *          read by antidiagonals.
 * https://oeis.org/A004481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   T(r,c) = mex of T(r-k,c), T(r,c-k), T(r-k,c-k) for k >= 1
 *            (Sprague-Grundy values of Wythoff's game); computed by dynamic
 *            programming over the table, read by antidiagonals
 * Range:     u8 n <= 5049, u16 n <= 5049, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static int *grundy_tab = NULL;
static long long grundy_size = 0;

static void build_table(long long sz) {
    int *tab = (int *)malloc((size_t)(sz * sz) * sizeof(int));
    long long maxopt = 3 * sz + 2;
    char *mark = (char *)malloc((size_t)maxopt);
    long long r, c, k;
    for (r = 0; r < sz; r++) {
        for (c = 0; c < sz; c++) {
            long long m;
            int g;
            memset(mark, 0, (size_t)maxopt);
            for (k = 1; k <= r; k++) mark[tab[(r - k) * sz + c]] = 1;
            for (k = 1; k <= c; k++) mark[tab[r * sz + (c - k)]] = 1;
            m = r < c ? r : c;
            for (k = 1; k <= m; k++) mark[tab[(r - k) * sz + (c - k)]] = 1;
            g = 0;
            while (mark[g]) g++;
            tab[r * sz + c] = g;
        }
    }
    free(mark);
    if (grundy_tab) free(grundy_tab);
    grundy_tab = tab;
    grundy_size = sz;
}

seq_t a(idx_t n) {
    idx_t d = 0;
    idx_t i;
    long long row, col, need;
    while ((d + 1) * (d + 2) / 2 <= n) d++;
    i = n - d * (d + 1) / 2;
    row = (long long)(d - i);
    col = (long long)i;
    need = (long long)d + 1;
    if (need > grundy_size) {
        long long sz = grundy_size * 2;
        if (sz < need) sz = need;
        if (sz < 16) sz = 16;
        build_table(sz);
    }
    return (seq_t)grundy_tab[row * grundy_size + col];
}
