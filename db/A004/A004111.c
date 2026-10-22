/*
 * A004111  Number of rooted identity trees with n nodes (rooted trees whose
 *          automorphism group is the identity group).
 * https://oeis.org/A004111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, eigen
 * Formula:   A(x) = Sum_{n>=1} a(n)*x^n = x * Product_{n>=1} (1+x^n)^a(n),
 *            a(0) = 0, a(1) = 1; equivalently a(n+1) = (1/n) * Sum_{k=1..n} (
 *            Sum_{d|k} (-1)^(k/d+1) d*a(d) ) * a(n-k+1). Fixed-width builds
 *            use the division-free product (binomials of a(k) computed
 *            exactly mod 2^w); the arbitrary-precision build u...
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 30, u64 n <= 55, u128 n <= 104,
 *            big any n
 * Helpers:   binom_c, div_one, grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/binom_c.h"
#include "../utils/div_one.h"

#define FIX_CAP 200

static seq_t *fx_a = NULL;

static void build_fixed(void) {
    seq_t *P = grow_zero(NULL, 0, FIX_CAP + 1);
    seq_t *Q = grow_zero(NULL, 0, FIX_CAP + 1);
    seq_t *B = grow_zero(NULL, 0, FIX_CAP + 1);
    seq_t *t = grow_zero(NULL, 0, FIX_CAP + 1);
    idx_t k, i, j, jm;
    fx_a = grow_zero(NULL, 0, FIX_CAP + 1);
    P[0] = 1;
    fx_a[0] = 0;
    for (k = 1; k <= FIX_CAP; k++) {
        fx_a[k] = P[k - 1];
        jm = FIX_CAP / k;
        for (j = 0; j <= jm; j++) B[j] = binom_c(fx_a[k], j, t);
        for (i = 0; i <= FIX_CAP; i++) Q[i] = 0;
        for (i = 0; i <= FIX_CAP; i++) {
            if (P[i] == 0) continue;
            for (j = 0; j <= jm && i + j * k <= FIX_CAP; j++)
                Q[i + j * k] += P[i] * B[j];
        }
        for (i = 0; i <= FIX_CAP; i++) P[i] = Q[i];
    }
    free(P); free(Q); free(B); free(t);
}

static seq_t *bA = NULL, *bCP = NULL, *bCN = NULL;
static idx_t b_have = 0, b_cap = 0;

static void add_c(idx_t m) {
    idx_t d, e;
    for (d = 1; d * d <= m; d++) {
        if (m % d != 0) continue;
        e = m / d;
        if (e % 2 == 1) bCP[m] += (seq_t)d * bA[d]; else bCN[m] += (seq_t)d * bA[d];
        if (e != d) {
            if (d % 2 == 1) bCP[m] += (seq_t)e * bA[e]; else bCN[m] += (seq_t)e * bA[e];
        }
    }
}

static void ensure_big(idx_t n) {
    idx_t m, k;
    if (n + 1 > b_cap) {
        idx_t nc = (n + 1) * 2;
        bA = grow_zero(bA, b_cap, nc);
        bCP = grow_zero(bCP, b_cap, nc);
        bCN = grow_zero(bCN, b_cap, nc);
        b_cap = nc;
    }
    for (m = b_have; m <= n; m++) {
        if (m == 0) { bA[0] = 0; continue; }
        if (m == 1) { bA[1] = 1; add_c(1); continue; }
        {
            seq_t pos = 0, neg = 0;
            for (k = 1; k <= m - 1; k++) {
                pos += bCP[k] * bA[m - k];
                neg += bCN[k] * bA[m - k];
            }
            bA[m] = (pos - neg) / (seq_t)(m - 1);
        }
        add_c(m);
    }
    if (n + 1 > b_have) b_have = n + 1;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        if (n > FIX_CAP) return 0; /* far beyond any fixed-width range */
        if (fx_a == NULL) build_fixed();
        return fx_a[n];
    }
    if (n >= b_have) ensure_big(n);
    return bA[n];
}
