/*
 * A004104  Number of self-dual signed graphs with n nodes. Also number of
 *          self-complementary 2-multigraphs on n nodes.
 * https://oeis.org/A004104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (1/n!) * Sum_{sigma in S_n} 3^(number of even-length
 *            cycles of sigma acting on 2-subsets of nodes), summed over cycle
 *            types: even parts are enumerated explicitly, odd parts are
 *            summed by the EGF recurrence g(j) = Sum_{b odd <= j}
 *            (j-1)!/(j-b)! * 3^(Sum_{even parts e} gcd(e,b)) * g(j-b), g(...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; int len; } bn;

static int bn_cap;

static void bn_init(bn *x) { x->d = (uint32_t *)calloc((size_t)bn_cap, sizeof(uint32_t)); x->len = 0; }
static void bn_set(bn *x, uint32_t v) { x->len = 0; if (v) { x->d[0] = v; x->len = 1; } }
static void bn_copy(bn *x, const bn *y) {
    if (y->len) memcpy(x->d, y->d, (size_t)y->len * sizeof(uint32_t));
    x->len = y->len;
}
static void bn_muls(bn *x, uint32_t m) {
    uint64_t c = 0; int i;
    if (m == 0) { x->len = 0; return; }
    for (i = 0; i < x->len; i++) { c += (uint64_t)x->d[i] * m; x->d[i] = (uint32_t)c; c >>= 32; }
    if (c) x->d[x->len++] = (uint32_t)c;
}
static void bn_divs(bn *x, uint32_t m) {
    uint64_t r = 0; int i;
    for (i = x->len - 1; i >= 0; i--) { r = (r << 32) | x->d[i]; x->d[i] = (uint32_t)(r / m); r %= m; }
    while (x->len && x->d[x->len - 1] == 0) x->len--;
}
static void bn_add(bn *x, const bn *y) {
    uint64_t c = 0; int i;
    while (x->len < y->len) x->d[x->len++] = 0;
    for (i = 0; i < x->len; i++) {
        c += x->d[i];
        if (i < y->len) c += y->d[i];
        x->d[i] = (uint32_t)c; c >>= 32;
        if (i >= y->len && !c) break;
    }
    if (c) x->d[x->len++] = (uint32_t)c;
}
static void bn_mul(bn *z, const bn *x, const bn *y) {
    int i, j, lx = x->len, ly = y->len;
    if (!lx || !ly) { z->len = 0; return; }
    memset(z->d, 0, (size_t)(lx + ly) * sizeof(uint32_t));
    for (i = 0; i < lx; i++) {
        uint64_t c = 0, xi = x->d[i];
        for (j = 0; j < ly; j++) {
            c += xi * y->d[j] + z->d[i + j];
            z->d[i + j] = (uint32_t)c; c >>= 32;
        }
        z->d[i + ly] = (uint32_t)c;
    }
    z->len = lx + ly;
    while (z->len && z->d[z->len - 1] == 0) z->len--;
}
static void bn_pow3(bn *x, long e) {
    while (e >= 20) { bn_muls(x, 3486784401u); e -= 20; }
    if (e > 0) { uint32_t p = 1; while (e--) p *= 3; bn_muls(x, p); }
}

static int gcdi(int x, int y) { while (y) { int t = x % y; x = y; y = t; } return x; }

static int N;
static long *xo;
static long eE;
static int *parts;
static int np;
static bn *coefs;
static bn *g;
static bn tmp, P, T;

static void leaf(const bn *coef, int s) {
    int m = N - s, j, b;
    bn_set(&g[0], 1);
    for (j = 1; j <= m; j++) {
        bn_set(&g[j], 0);
        bn_set(&P, 1);
        for (b = 1; b <= j; b += 2) {
            bn_mul(&tmp, &P, &g[j - b]);
            bn_pow3(&tmp, xo[b]);
            bn_add(&g[j], &tmp);
            if (b + 2 <= j) { bn_muls(&P, (uint32_t)(j - b)); bn_muls(&P, (uint32_t)(j - b - 1)); }
        }
    }
    bn_mul(&tmp, coef, &g[m]);
    bn_pow3(&tmp, eE);
    bn_add(&T, &tmp);
}

static void dfs(int maxa, int s, int depth) {
    int a, c, k, b;
    leaf(&coefs[depth], s);
    for (a = maxa; a >= 2; a -= 2) {
        int d = depth, cs = s, savenp = np;
        long saveE = eE;
        for (c = 1; cs + a <= N; c++) {
            bn_copy(&coefs[d + 1], &coefs[d]);
            for (k = 0; k < a; k++) bn_muls(&coefs[d + 1], (uint32_t)(N - cs - k));
            bn_divs(&coefs[d + 1], (uint32_t)a);
            bn_divs(&coefs[d + 1], (uint32_t)c);
            eE += (a - 2) / 2 + (a % 4 == 0 ? 1 : 0);
            for (k = 0; k < np; k++) eE += gcdi(a, parts[k]);
            parts[np++] = a;
            for (b = 1; b <= N; b += 2) xo[b] += gcdi(a, b);
            cs += a; d++;
            dfs(a - 2, cs, d);
        }
        for (b = 1; b <= N; b += 2) xo[b] -= (long)(c - 1) * gcdi(a, b);
        eE = saveE; np = savenp;
    }
}

seq_t a(idx_t n) {
    int nn = (int)n, i, ncoef;
    seq_t r = 0;
    N = nn;
    bn_cap = (nn * nn + 20 * nn + 64) / 32 + 4;
    xo = (long *)calloc((size_t)nn + 2, sizeof(long));
    parts = (int *)malloc(((size_t)nn + 1) * sizeof(int));
    np = 0; eE = 0;
    ncoef = nn / 2 + 2;
    coefs = (bn *)malloc((size_t)ncoef * sizeof(bn));
    for (i = 0; i < ncoef; i++) bn_init(&coefs[i]);
    g = (bn *)malloc(((size_t)nn + 1) * sizeof(bn));
    for (i = 0; i <= nn; i++) bn_init(&g[i]);
    bn_init(&tmp); bn_init(&P); bn_init(&T);

    bn_set(&coefs[0], 1);
    bn_set(&T, 0);
    dfs(nn - (nn % 2), 0, 0);
    for (i = 2; i <= nn; i++) bn_divs(&T, (uint32_t)i);

    for (i = T.len - 1; i >= 0; i--) {
        r = (seq_t)(r << 8); r = (seq_t)(r << 8);
        r = (seq_t)(r << 8); r = (seq_t)(r << 8);
        r += (seq_t)T.d[i];
    }

    for (i = 0; i < ncoef; i++) free(coefs[i].d);
    for (i = 0; i <= nn; i++) free(g[i].d);
    free(coefs); free(g);
    free(tmp.d); free(P.d); free(T.d);
    free(xo); free(parts);
    return r;
}
