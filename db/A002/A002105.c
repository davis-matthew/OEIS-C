/*
 * A002105  Reduced tangent numbers: 2^n*(2^{2n} - 1)*|B_{2n}|/n, where B_n =
 *          Bernoulli numbers.
 * https://oeis.org/A002105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   Poupard's triangle (A008301). Row m has 2m-1 symmetric entries
 *            d_m(0..2m-2), with d_1(0) = 1. Row m+1 starts with d(0) = a(m+1)
 *            = sum of row m and d(1) = 2*d(0), and then d(j+1) = 2*d(j) -
 *            d(j-1) - 2*d_m(j-1). a(1) = 1, and a(n) = d_n(0)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   a2105_zalloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/a2105_zalloc.h"

static seq_t *vals = NULL;
static seq_t *rowA = NULL;
static seq_t *rowB = NULL;
static idx_t cur = 0;
static idx_t cap = 0;

static void a2105_grow(idx_t newcap) {
    seq_t *nv = a2105_zalloc(newcap);
    seq_t *na = a2105_zalloc(newcap);
    seq_t *nb = a2105_zalloc(newcap);
    if (vals) {
        for (idx_t i = 0; i <= cur; i++) nv[i] = vals[i];
        for (idx_t i = 0; i < cur; i++) na[i] = rowA[i];
        free(vals);
        free(rowA);
        free(rowB);
    }
    vals = nv;
    rowA = na;
    rowB = nb;
    cap = newcap;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (cur == 0) {
        a2105_grow(n + 1 > 16 ? n + 1 : 16);
        vals[1] = 1;
        rowA[0] = 1;
        cur = 1;
    }
    if (n >= cap) {
        idx_t nc = cap * 2;
        if (nc < n + 1) nc = n + 1;
        a2105_grow(nc);
    }
    while (cur < n) {
        idx_t m = cur + 1;
        seq_t s = 0;
        for (idx_t j = 0; j + 1 < cur; j++) s += rowA[j];
        s = s * 2 + rowA[cur - 1];
        vals[m] = s;
        rowB[0] = s;
        rowB[1] = s * 2;
        for (idx_t j = 1; j + 1 < m; j++) {
            rowB[j + 1] = rowB[j] * 2 - rowB[j - 1] - rowA[j - 1] * 2;
        }
        seq_t *t = rowA;
        rowA = rowB;
        rowB = t;
        cur = m;
    }
    return vals[n];
}
