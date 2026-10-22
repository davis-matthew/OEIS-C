/*
 * A002500  Number of self-converse relations on n points.
 * https://oeis.org/A002500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2*A029849(n) - A000595(n) = (1/n!) * Sum_{sigma in S_n}
 *            2^c(sigma), where c(sigma) is the number of cycles of (i,j) ->
 *            (sigma(j), sigma(i)) on ordered pairs (de Bruijn / Burnside
 *            count of relations isomorphic to their converse); summed over
 *            cycle types
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   inv128, sg
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/inv128.h"
#include "../utils/sg.h"

static idx_t *cs_tab = 0;
static idx_t cs_len = 0;

static idx_t cself(idx_t p) {
    if (p < cs_len && cs_tab[p]) return cs_tab[p];
    if (p >= cs_len) {
        idx_t nl = p + 16, i;
        idx_t *t = (idx_t *)malloc(nl * sizeof(idx_t));
        for (i = 0; i < nl; i++) t[i] = (i < cs_len) ? cs_tab[i] : 0;
        free(cs_tab);
        cs_tab = t;
        cs_len = nl;
    }
    {
        unsigned char *vis = (unsigned char *)calloc((size_t)(p * p), 1);
        idx_t cnt = 0, s;
        for (s = 0; s < p * p; s++) {
            if (vis[s]) continue;
            cnt++;
            {
                idx_t cur = s;
                while (!vis[cur]) {
                    idx_t i = cur / p, j = cur % p;
                    vis[cur] = 1;
                    cur = ((j + 1) % p) * p + ((i + 1) % p);
                }
            }
        }
        free(vis);
        cs_tab[p] = cnt;
    }
    return cs_tab[p];
}

static int g_big;
static idx_t *g_p, *g_m;
static uint64_t f_nf, f_R;
static unsigned __int128 f_Q;
static seq_t b_S, b_nf;

static uint64_t pow2mod(idx_t e, uint64_t m) {
    unsigned __int128 r = 1 % m, b = 2 % m;
    while (e) {
        if (e & 1) r = (r * b) % m;
        b = (b * b) % m;
        e >>= 1;
    }
    return (uint64_t)r;
}

static void leaf(idx_t c, uint64_t z, seq_t zs) {
    if (g_big) {
        b_S += ((seq_t)1 << c) * (b_nf / zs);
    } else {
        unsigned v = 0;
        uint64_t zo = z, r;
        unsigned __int128 q;
        while (!(zo & 1)) { zo >>= 1; v++; }
        r = pow2mod(c, z);
        if (c < v) {
            q = 0;
        } else {
            unsigned __int128 P = (c - v >= 128) ? 0 : ((unsigned __int128)1 << (c - v));
            q = (P - (unsigned __int128)(r >> v)) * inv128(zo);
        }
        f_Q += q;
        f_R += r * (f_nf / z);
        if (f_R >= f_nf) { f_R -= f_nf; f_Q++; }
    }
}

static void rec(idx_t rem, idx_t maxp, idx_t k, idx_t c, uint64_t z, seq_t zs) {
    idx_t p;
    if (rem == 0) { leaf(c, z, zs); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        idx_t pg = (p % 2 == 0) ? 2 : 1, pp = p / pg;
        idx_t cross = 0, i, m, cc = c;
        uint64_t zz = z;
        seq_t zb = zs;
        idx_t csp = cself(p);
        for (i = 0; i < k; i++) {
            idx_t q = g_p[i], qg = (q % 2 == 0) ? 2 : 1, qp = q / qg;
            cross += g_m[i] * (p * q / (pp * qp) * sg(pp, qp));
        }
        for (m = 1; m * p <= rem; m++) {
            cc += csp + (m - 1) * p * pg + cross;
            if (g_big) zb = zb * (seq_t)(p * m);
            else zz *= (uint64_t)(p * m);
            g_p[k] = p;
            g_m[k] = m;
            rec(rem - m * p, p - 1, k + 1, cc, zz, zb);
        }
    }
}

seq_t a(idx_t n) {
    seq_t t = 1, res;
    int i;
    idx_t j;
    for (i = 0; i < 200; i++) t += t;
    g_big = (t != 0);
    if (!g_big && n > 20) return 0;
    g_p = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    g_m = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (g_big) {
        b_nf = 1;
        for (j = 2; j <= n; j++) b_nf *= (seq_t)j;
        b_S = 0;
        rec(n, n, 0, 0, 1, (seq_t)1);
        res = b_S / b_nf;
    } else {
        uint64_t hi, lo;
        f_nf = 1;
        for (j = 2; j <= n; j++) f_nf *= (uint64_t)j;
        f_Q = 0;
        f_R = 0;
        rec(n, n, 0, 0, 1, (seq_t)1);
        hi = (uint64_t)(f_Q >> 64);
        lo = (uint64_t)f_Q;
        res = (seq_t)hi;
        for (i = 0; i < 64; i++) res += res;
        res += (seq_t)lo;
    }
    free(g_p);
    free(g_m);
    return res;
}
