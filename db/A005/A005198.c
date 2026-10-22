/*
 * A005198  a(n) is the number of forests with n (unlabeled) nodes in which
 *          each component tree is planted, that is, is a rooted tree in which
 *          the root has degree 1.
 * https://oeis.org/A005198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the Euler transform of b(k) = A000081(k-1) for k >= 2,
 *            b(1) = 0: a(0) = 1, a(n) = (1/n) * Sum_{k=1..n} c(k)*a(n-k),
 *            c(k) = Sum_{d|k, d>=2} d*A000081(d-1); A000081(1) = 1,
 *            A000081(m) = (1/(m-1)) * Sum_{k=1..m-1} (Sum_{d|k} d*A000081(d))
 *            * A000081(m-k). For fixed-width seq_t, the same trans...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 47, u128 n <= 89,
 *            big any n
 * Helpers:   fold, grow_zero, my_gcd, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/fold.h"
#include "../utils/my_gcd.h"

static seq_t *R = NULL, *CR = NULL, *CB = NULL, *AV = NULL;
static idx_t cap = 0, have = 0;

static void ensure(idx_t n) {
    if (n + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 16) nc = 16;
        R = grow_zero(R, cap, nc);
        CR = grow_zero(CR, cap, nc);
        CB = grow_zero(CB, cap, nc);
        AV = grow_zero(AV, cap, nc);
        if (cap == 0) { AV[0] = 1; R[0] = 0; }
        cap = nc;
    }
    for (idx_t m = have + 1; m <= n; m++) {
        if (m == 1) {
            R[1] = 1;
        } else {
            seq_t s = 0;
            for (idx_t k = 1; k < m; k++) s += CR[k] * R[m - k];
            R[m] = s / (seq_t)(m - 1);
        }
        seq_t cr = 0, cb = 0;
        for (idx_t d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t e = m / d;
                cr += (seq_t)d * R[d];
                if (d >= 2) cb += (seq_t)d * R[d - 1];
                if (e != d) {
                    cr += (seq_t)e * R[e];
                    if (e >= 2) cb += (seq_t)e * R[e - 1];
                }
            }
        }
        CR[m] = cr;
        CB[m] = cb;
        seq_t s = 0;
        for (idx_t k = 1; k <= m; k++) s += CB[k] * AV[m - k];
        AV[m] = s / (seq_t)m;
        have = m;
    }
}

static seq_t fixed_a(idx_t n) {
    seq_t *r = (seq_t *)malloc(sizeof(seq_t) * (n + 2));
    seq_t *F = (seq_t *)malloc(sizeof(seq_t) * (n + 2));
    seq_t *G = (seq_t *)malloc(sizeof(seq_t) * (n + 2));
    seq_t *binc = (seq_t *)malloc(sizeof(seq_t) * (n + 2));
    for (idx_t i = 0; i <= n + 1; i++) { r[i] = 0; F[i] = 0; G[i] = 0; binc[i] = 0; }
    F[0] = 1;
    G[0] = 1;
    for (idx_t k = 1; k <= n; k++) {
        if (k == 1) r[1] = 1;
        else r[k] = F[k - 1];
        fold(F, binc, n, k, r[k]);
        if (k >= 2) fold(G, binc, n, k, r[k - 1]);
    }
    seq_t res = G[n];
    free(r); free(F); free(G); free(binc);
    return res;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    if (seq_is_fixed_width() && n <= 400) return fixed_a(n);
    ensure(n);
    return AV[n];
}
