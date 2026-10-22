/*
 * A007297  Number of connected graphs on n labeled nodes on a circle with
 *          straight-line edges that don't cross.
 * https://oeis.org/A007297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1) = 1 and a(n) = c(n-1) for n >= 2, where y = Sum_{m>=1} c(m)
 *            x^m is the series reversion of (x-x^2)/(1+x)^3, i.e. y =
 *            x*(1+y)^3 + y^2, so c(m) = [m=1] + 3*c(m-1) + 3*[x^(m-1)]y^2 +
 *            [x^(m-1)]y^3 + [x^m]y^2 (needs only + - *, so it is exact modulo
 *            2^w in fixed-width builds). In the arbitrary-pr...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_big.h"
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *cc = NULL, *SS = NULL, *TT = NULL;
static idx_t cap = 0;
static idx_t done = 0;

static void ensure(idx_t need) {
    if (need <= cap) return;
    idx_t nc = cap * 2;
    if (nc < need) nc = need;
    if (nc < 16) nc = 16;
    if (cap == 0) {
        cc = alloc_zero(nc);
        SS = alloc_zero(nc);
        TT = alloc_zero(nc);
    } else {
        cc = grow_zero(cc, cap, nc);
        SS = grow_zero(SS, cap, nc);
        TT = grow_zero(TT, cap, nc);
    }
    cap = nc;
}

seq_t a(idx_t n) {
    if (n <= 2) return 1;
    ensure(n + 1);
    if (seq_is_big()) {
        if (done == 0) {
            cc[1] = 1;
            cc[2] = 1;
            done = 3;
        }
        for (idx_t i = done; i <= n; i++) {
            seq_t is = (seq_t)i;
            seq_t i1 = is - 1;
            seq_t i3 = 3 * is;
            seq_t t1 = 12 * (i3 - 8) * (i3 - 1) * (i3 - 7) * cc[i - 2];
            seq_t t2 = 36 * i1 * cc[i - 1];
            cc[i] = (t1 - t2) / (is * i1 * (i3 - 4));
        }
        if (n + 1 > done) done = n + 1;
        return cc[n];
    } else {
        if (done == 0) done = 1;
        for (idx_t m = done; m < n; m++) {
            seq_t s = 0;
            for (idx_t i = 1; i < m; i++) s += cc[i] * cc[m - i];
            SS[m] = s;
            seq_t t = 0;
            for (idx_t i = 1; i + 1 < m; i++) t += cc[i] * SS[m - 1 - i];
            TT[m - 1] = t;
            seq_t v = 3 * cc[m - 1] + 3 * SS[m - 1] + TT[m - 1] + SS[m];
            if (m == 1) v += 1;
            cc[m] = v;
        }
        if (n > done) done = n;
        return cc[n - 1];
    }
}
