/*
 * A002991  Number of n-node trees with a forbidden limb of length 5.
 * https://oeis.org/A002991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: 1 + V(x) - (E(x) - E_sym(x))/2 (dissymmetry theorem),
 *            where G(x) = x*MSET(G)(x) - x^5 counts valid planted subtrees (G
 *            is the g.f. of A052328), P(x) = x*(MSET(G) - 1 - G) counts
 *            planted subtrees whose root branches, B(x) = P(x)/(1-x). V(x)
 *            counts vertex-rooted trees and E(x) counts edge-roo...
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 28, u64 n <= 50, u128 n <= 93,
 *            big any n
 * Helpers:   A_grow, w2991_is_fixed, w2991_pairs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/A_grow.h"
#include "../utils/w2991_is_fixed.h"
#include "../utils/w2991_pairs.h"

#define W2991_FL 8
#define W2991_FMAX 256

typedef struct { uint32_t d[W2991_FL]; } w2991_n;

static seq_t *w2991_G = 0, *w2991_P = 0, *w2991_B = 0, *w2991_M = 0, *w2991_C = 0;
static idx_t w2991_have = 0;
static int w2991_mode = -1;

static void w2991_set(w2991_n *r, uint32_t v) {
    int i;
    for (i = 0; i < W2991_FL; i++) r->d[i] = 0;
    r->d[0] = v;
}

static void w2991_addto(w2991_n *r, const w2991_n *a) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < W2991_FL; i++) {
        c += (uint64_t)r->d[i] + a->d[i];
        r->d[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void w2991_subfrom(w2991_n *r, const w2991_n *a) {
    uint64_t br = 0;
    int i;
    for (i = 0; i < W2991_FL; i++) {
        uint64_t t = (uint64_t)r->d[i] - a->d[i] - br;
        r->d[i] = (uint32_t)t;
        br = (t >> 32) & 1;
    }
}

static void w2991_addmul(w2991_n *r, const w2991_n *a, const w2991_n *b) {
    w2991_n t;
    int i, j;
    for (i = 0; i < W2991_FL; i++) t.d[i] = 0;
    for (i = 0; i < W2991_FL; i++) {
        uint64_t carry = 0;
        if (a->d[i] == 0) continue;
        for (j = 0; i + j < W2991_FL; j++) {
            uint64_t cur = (uint64_t)a->d[i] * b->d[j] + t.d[i + j] + carry;
            t.d[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
    }
    w2991_addto(r, &t);
}

static void w2991_mulsmall(w2991_n *r, uint32_t k) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < W2991_FL; i++) {
        c += (uint64_t)r->d[i] * k;
        r->d[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void w2991_divsmall(w2991_n *r, uint32_t dv) {
    uint64_t rem = 0;
    int i;
    for (i = W2991_FL - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | r->d[i];
        r->d[i] = (uint32_t)(cur / dv);
        rem = cur % dv;
    }
}

static seq_t w2991_conv(const w2991_n *x) {
    seq_t r = 0;
    int i;
    for (i = W2991_FL - 1; i >= 0; i--) {
        r = r * (seq_t)65536;
        r = r * (seq_t)65536;
        r += (seq_t)x->d[i];
    }
    return r;
}

static w2991_n w2991_FM[W2991_FMAX + 1], w2991_FG[W2991_FMAX + 1], w2991_FC[W2991_FMAX + 1];

static void w2991_fixed_fill(void) {
    idx_t n, k, d, len = W2991_FMAX + 1;
    w2991_n one, t;
    w2991_set(&one, 1);
    w2991_set(&w2991_FM[0], 1);
    w2991_set(&w2991_FG[0], 0);
    w2991_set(&w2991_FC[0], 0);
    for (n = 1; n < len; n++) {
        w2991_FG[n] = w2991_FM[n - 1];
        if (n == 5) w2991_subfrom(&w2991_FG[n], &one);
        w2991_set(&w2991_FC[n], 0);
        for (d = 1; d <= n; d++) {
            if (n % d) continue;
            t = w2991_FG[d];
            w2991_mulsmall(&t, (uint32_t)d);
            w2991_addto(&w2991_FC[n], &t);
        }
        w2991_set(&w2991_FM[n], 0);
        for (k = 1; k <= n; k++)
            w2991_addmul(&w2991_FM[n], &w2991_FC[k], &w2991_FM[n - k]);
        w2991_divsmall(&w2991_FM[n], (uint32_t)n);
    }
    w2991_G = A_grow(w2991_G, 0, len);
    w2991_P = A_grow(w2991_P, 0, len);
    w2991_B = A_grow(w2991_B, 0, len);
    w2991_G[0] = 0;
    w2991_P[0] = 0;
    w2991_B[0] = 0;
    for (n = 1; n < len; n++) {
        w2991_G[n] = w2991_conv(&w2991_FG[n]);
        t = w2991_FM[n - 1];
        w2991_subfrom(&t, &w2991_FG[n - 1]);
        if (n == 1) w2991_subfrom(&t, &one);
        w2991_P[n] = w2991_conv(&t);
        w2991_B[n] = w2991_B[n - 1] + w2991_P[n];
    }
    w2991_have = len;
}

static void w2991_big_extend(idx_t N) {
    idx_t nl, n, k, d;
    if (N < w2991_have) return;
    nl = N + 1;
    if (nl < w2991_have + 64) nl = w2991_have + 64;
    w2991_G = A_grow(w2991_G, w2991_have, nl);
    w2991_P = A_grow(w2991_P, w2991_have, nl);
    w2991_B = A_grow(w2991_B, w2991_have, nl);
    w2991_M = A_grow(w2991_M, w2991_have, nl);
    w2991_C = A_grow(w2991_C, w2991_have, nl);
    for (n = w2991_have; n < nl; n++) {
        seq_t c = 0, s = 0;
        if (n == 0) {
            w2991_M[0] = 1; w2991_G[0] = 0; w2991_C[0] = 0;
            w2991_P[0] = 0; w2991_B[0] = 0;
            continue;
        }
        w2991_G[n] = w2991_M[n - 1];
        if (n == 5) w2991_G[n] -= 1;
        for (d = 1; d * d <= n; d++) {
            if (n % d) continue;
            c += w2991_G[d] * (seq_t)d;
            if (n / d != d) c += w2991_G[n / d] * (seq_t)(n / d);
        }
        w2991_C[n] = c;
        for (k = 1; k <= n; k++) s += w2991_C[k] * w2991_M[n - k];
        w2991_M[n] = s / (seq_t)n;
        w2991_P[n] = w2991_M[n - 1] - w2991_G[n - 1];
        if (n == 1) w2991_P[n] -= 1;
        w2991_B[n] = w2991_B[n - 1] + w2991_P[n];
    }
    w2991_have = nl;
}

static seq_t w2991_pv(long long i) {
    if (i < 1) return (seq_t)0;
    return w2991_P[i];
}

seq_t a(idx_t n) {
    long long L, m;
    seq_t v = 0, h = 0;
    if (n == 0) return 1;
    if (w2991_mode < 0) {
        w2991_mode = w2991_is_fixed();
        if (w2991_mode) w2991_fixed_fill();
    }
    if (w2991_mode) {
        if (n > W2991_FMAX) return 0;
    } else {
        w2991_big_extend(n);
    }
    L = (long long)n;
    m = L - 1;

    /* vertex-rooted */
    if (L == 1) v += 1;
    v += w2991_P[L];                      /* root degree >= 3 (with subtraction below) */
    /* root degree 2 */
    v += w2991_pairs(w2991_B, m, 0);
    v += w2991_pv(L - 2) + w2991_pv(L - 3) * 2 + w2991_pv(L - 4) * 3;
    if (m == 2 || m == 3) v += 1;
    if (m == 4) v += 2;
    /* root degree 1 */
    if (m >= 1 && m <= 4) v += 1;
    v += w2991_pv(L - 1) + w2991_pv(L - 2) + w2991_pv(L - 3) + w2991_pv(L - 4);
    v -= w2991_pairs(w2991_G, m, 0);

    /* edge-rooted, nonsymmetric unordered */
    h += w2991_pairs(w2991_B, L, 1);
    h += w2991_pv(L - 1) + w2991_pv(L - 2) * 2 + w2991_pv(L - 3) * 3 + w2991_pv(L - 4) * 4;
    if (L == 3 || L == 4) h += 1;
    if (L == 5) h += 2;

    return v - h;
}
