/*
 * A007860  Maximal matchings in rooted plane trees on n nodes.
 * https://oeis.org/A007860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A_n + B_n where, with s = A + B, D = 1/(1-s), E =
 *            1/(1-A): A = x^2*D^3 (root matched to a child), B = x*E (root
 *            unmatched, all children matched). Coefficientwise: A_n =
 *            [x^(n-2)] D^3, B_n = E_(n-1), E_0 = D_0 = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"

static seq_t *cA = NULL, *cE = NULL, *cS = NULL, *cD = NULL, *cD2 = NULL, *cD3 = NULL;
static idx_t cap = 0;   /* allocated length */
static idx_t done = 0;  /* coefficients 0..done-1 are computed */

static void ensure(idx_t n) {
    if (n + 1 <= done) return;
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 64;
        while (nc < n + 1) nc *= 2;
        cA = grow_zero(cA, cap, nc);
        cE = grow_zero(cE, cap, nc);
        cS = grow_zero(cS, cap, nc);
        cD = grow_zero(cD, cap, nc);
        cD2 = grow_zero(cD2, cap, nc);
        cD3 = grow_zero(cD3, cap, nc);
        cap = nc;
    }
    for (idx_t k = done; k <= n; k++) {
        if (k == 0) {
            cA[0] = 0; cS[0] = 0;
            cE[0] = 1; cD[0] = 1; cD2[0] = 1; cD3[0] = 1;
            continue;
        }
        seq_t av = 0;
        if (k >= 2) av = cD3[k - 2];
        cA[k] = av;
        seq_t e = 0;
        for (idx_t i = 1; i <= k; i++) e += cA[i] * cE[k - i];
        cE[k] = e;
        seq_t bv = cE[k - 1];
        cS[k] = av + bv;
        seq_t d = 0;
        for (idx_t i = 1; i <= k; i++) d += cS[i] * cD[k - i];
        cD[k] = d;
        seq_t d2 = 0;
        for (idx_t i = 0; i <= k; i++) d2 += cD[i] * cD[k - i];
        cD2[k] = d2;
        seq_t d3 = 0;
        for (idx_t i = 0; i <= k; i++) d3 += cD2[i] * cD[k - i];
        cD3[k] = d3;
    }
    done = n + 1;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure(n);
    return cS[n];
}
