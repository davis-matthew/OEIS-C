/*
 * A001669  Number of 7-level labeled rooted trees with n leaves.
 * https://oeis.org/A001669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   E.g.f.: exp(exp(exp(exp(exp(exp(exp(x)-1)-1)-1)-1)-1)-1).
 *            Computed by iterating g = exp(f): g(0) = 1, g(m) = Sum_{k=1..m}
 *            binomial(m-1,k-1)*f(k)*g(m-k), seven times starting from f = x
 *            (the inner levels drop the constant term)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 138 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *a001028_z(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *a001028_grow(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = a001028_z(newlen);
    for (idx_t i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) mem_free(old);
    return p;
}

static seq_t *lv[7];
static seq_t *brow = 0;
static idx_t cap = 0;
static idx_t done = 0;   /* indices 0..done-1 computed */

seq_t a(idx_t n) {
    idx_t m, k;
    int L;
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < n + 1) nc *= 2;
        if (cap == 0) {
            for (L = 0; L < 7; L++) lv[L] = a001028_z(nc);
            brow = a001028_z(nc);
        } else {
            for (L = 0; L < 7; L++) lv[L] = a001028_grow(lv[L], cap, nc);
            brow = a001028_grow(brow, cap, nc);
        }
        cap = nc;
    }
    if (done == 0) {
        for (L = 0; L < 7; L++) lv[L][0] = 1;
        brow[0] = 1;          /* row 0 of Pascal's triangle */
        done = 1;
    }
    for (m = done; m <= n; m++) {
        /* brow currently holds row m-2 (for m>=2); advance to row m-1 */
        if (m >= 2) {
            idx_t r = m - 1;
            brow[r] = 1;
            for (k = r - 1; k >= 1; k--) brow[k] += brow[k - 1];
        }
        lv[0][m] = 1;  /* exp(x)-1 */
        for (L = 1; L < 7; L++) {
            seq_t s = 0;
            const seq_t *f = lv[L - 1];
            const seq_t *g = lv[L];
            for (k = 1; k <= m; k++) {
                s += brow[k - 1] * f[k] * g[m - k];
            }
            lv[L][m] = s;
        }
        done = m + 1;
    }
    return lv[6][n];
}
