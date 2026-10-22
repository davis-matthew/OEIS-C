/*
 * A007126  Number of connected rooted strength 1 Eulerian graphs with n
 *          nodes.
 * https://oeis.org/A007126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000088(n-1) - Sum_{k=1..n-1} a(k)*A002854(n-k), n>1,
 *            a(1) = 1. A000088(m) and A002854(m) are computed by Burnside's
 *            lemma over the cycle types of S_m. Cycle types are split into
 *            parts >= 3, handled one partition at a time, and the 2-cycles
 *            and fixed points, which are added in closed form
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 12, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   ig
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ig.h"

typedef struct { uint32_t *d; int len; int cap; } bn_t;

static void bn_reserve(bn_t *x, int cap) {
    if (x->cap < cap) {
        int nc = cap * 2 + 4;
        x->d = (uint32_t *)realloc(x->d, (size_t)nc * sizeof(uint32_t));
        x->cap = nc;
    }
}
static void bn_trim(bn_t *x) { while (x->len > 0 && x->d[x->len - 1] == 0) x->len--; }
static void bn_set_u(bn_t *x, uint32_t v) { bn_reserve(x, 1); x->d[0] = v; x->len = v ? 1 : 0; }
static void bn_free(bn_t *x) { free(x->d); x->d = NULL; x->len = 0; x->cap = 0; }
static void bn_copy(bn_t *dst, const bn_t *src) {
    bn_reserve(dst, src->len + 1);
    if (src->len) memcpy(dst->d, src->d, (size_t)src->len * sizeof(uint32_t));
    dst->len = src->len;
}
static void bn_mul_small(bn_t *x, uint32_t m) {
    uint64_t c = 0;
    int i;
    if (x->len == 0) return;
    bn_reserve(x, x->len + 1);
    for (i = 0; i < x->len; i++) {
        c += (uint64_t)x->d[i] * m;
        x->d[i] = (uint32_t)c;
        c >>= 32;
    }
    if (c) x->d[x->len++] = (uint32_t)c;
}
static void bn_div_small(bn_t *x, uint32_t m) {
    uint64_t r = 0;
    int i;
    for (i = x->len - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(cur / m);
        r = cur % m;
    }
    bn_trim(x);
}
/* x += y << s, in place */
static void bn_addsh(bn_t *x, const bn_t *y, long s) {
    int w, b, i, top;
    uint64_t c = 0;
    if (y->len == 0) return;
    w = (int)(s >> 5);
    b = (int)(s & 31);
    top = w + y->len + 1;
    if (top < x->len) top = x->len;
    bn_reserve(x, top + 1);
    for (i = x->len; i <= top; i++) x->d[i] = 0;
    for (i = 0; i <= y->len; i++) {
        uint64_t lo = i < y->len ? (((uint64_t)y->d[i] << b) & 0xFFFFFFFFULL) : 0;
        uint64_t hi = i > 0 ? (((uint64_t)y->d[i - 1] << b) >> 32) : 0;
        c += (uint64_t)x->d[w + i] + lo + hi;
        x->d[w + i] = (uint32_t)c;
        c >>= 32;
    }
    i = w + y->len + 1;
    while (c) {
        c += x->d[i];
        x->d[i] = (uint32_t)c;
        c >>= 32;
        i++;
    }
    x->len = top + 1;
    bn_trim(x);
}
static void bn_sub(bn_t *x, const bn_t *y) { /* requires x >= y */
    int64_t br = 0;
    int i;
    for (i = 0; i < x->len; i++) {
        int64_t v = (int64_t)x->d[i] - (i < y->len ? (int64_t)y->d[i] : 0) - br;
        if (v < 0) { v += ((int64_t)1 << 32); br = 1; } else br = 0;
        x->d[i] = (uint32_t)v;
    }
    bn_trim(x);
}
static void bn_mul(bn_t *r, const bn_t *a, const bn_t *b) {
    int i, j, n = a->len + b->len;
    if (a->len == 0 || b->len == 0) { r->len = 0; return; }
    bn_reserve(r, n + 1);
    for (i = 0; i < n; i++) r->d[i] = 0;
    for (i = 0; i < a->len; i++) {
        uint64_t c = 0;
        for (j = 0; j < b->len; j++) {
            c += (uint64_t)a->d[i] * b->d[j] + r->d[i + j];
            r->d[i + j] = (uint32_t)c;
            c >>= 32;
        }
        r->d[i + b->len] = (uint32_t)c;
    }
    r->len = n;
    bn_trim(r);
}

static int L_ = 0, D_o = 1, D_e = 1;
static int *gtab = NULL, *plen = NULL, *pmul = NULL;
static bn_t *wst = NULL, *G3 = NULL, *E3 = NULL;

/* partitions with all parts >= 3; weight L!/z, e = number of edge orbits */
static void dfs(int maxp, int t, int o, int ev, long e, int depth) {
    int i, id = (t * D_o + o) * D_e + ev;
    bn_addsh(&G3[id], &wst[depth], e);
    bn_addsh(&E3[id], &wst[depth], e - o - ev);
    for (i = (maxp < L_ - t ? maxp : L_ - t); i >= 3; i--) {
        bn_t *c = &wst[depth + 1];
        long ee = e, cross = 0;
        int mi, tt = t, k;
        for (k = 0; k < depth; k++) cross += (long)pmul[k] * gtab[plen[k] * (L_ + 1) + i];
        bn_copy(c, &wst[depth]);
        plen[depth] = i;
        for (mi = 1; tt + i <= L_; mi++) {
            tt += i;
            bn_div_small(c, (uint32_t)(i * mi));
            ee += i / 2 + (long)i * (mi - 1) + cross;
            pmul[depth] = mi;
            dfs(i - 1, tt, o + ((i & 1) ? mi : 0), ev + ((i & 1) ? 0 : mi), ee, depth + 1);
        }
    }
}

static bn_t *Gm = NULL, *Em = NULL, *Am = NULL;
static idx_t done = 0;

static void build(int M) {
    int L = M - 1, t, o, ev, C, f, k, s, n3, n2, DC;
    bn_t tg = {NULL, 0, 0}, te = {NULL, 0, 0};
    if (Gm) {
        for (k = 0; k <= (int)done; k++) { bn_free(&Gm[k]); bn_free(&Em[k]); bn_free(&Am[k]); }
        free(Gm); free(Em); free(Am);
    }
    Gm = (bn_t *)calloc((size_t)M + 1, sizeof(bn_t));
    Em = (bn_t *)calloc((size_t)M + 1, sizeof(bn_t));
    Am = (bn_t *)calloc((size_t)M + 1, sizeof(bn_t));
    if (L >= 1) {
        bn_t *G2, *E2;
        L_ = L; D_o = L / 3 + 1; D_e = L / 4 + 1; DC = L / 2 + 1;
        gtab = (int *)malloc((size_t)(L + 1) * (size_t)(L + 1) * sizeof(int));
        for (t = 0; t <= L; t++)
            for (o = 0; o <= L; o++) gtab[t * (L + 1) + o] = ig(t, o);
        n3 = (L + 1) * D_o * D_e;
        G3 = (bn_t *)calloc((size_t)n3, sizeof(bn_t));
        E3 = (bn_t *)calloc((size_t)n3, sizeof(bn_t));
        wst = (bn_t *)calloc((size_t)L + 2, sizeof(bn_t));
        plen = (int *)malloc(((size_t)L + 2) * sizeof(int));
        pmul = (int *)malloc(((size_t)L + 2) * sizeof(int));
        bn_set_u(&wst[0], 1);
        for (k = 2; k <= L; k++) bn_mul_small(&wst[0], (uint32_t)k);
        dfs(L, 0, 0, 0, 0, 0);
        for (k = 0; k < L + 2; k++) bn_free(&wst[k]);
        free(wst); free(plen); free(pmul); free(gtab);
        wst = NULL; plen = NULL; pmul = NULL; gtab = NULL;

        /* add 2-cycles */
        n2 = (L + 1) * DC * 2;
        G2 = (bn_t *)calloc((size_t)n2, sizeof(bn_t));
        E2 = (bn_t *)calloc((size_t)n2, sizeof(bn_t));
        for (t = 0; t <= L; t++)
            for (o = 0; o < D_o; o++)
                for (ev = 0; ev < D_e; ev++) {
                    int id = (t * D_o + o) * D_e + ev, k2;
                    if (G3[id].len == 0) continue;
                    bn_copy(&tg, &G3[id]);
                    bn_copy(&te, &E3[id]);
                    for (k2 = 0; t + 2 * k2 <= L; k2++) {
                        long shG;
                        int id2 = ((t + 2 * k2) * DC + (o + ev + k2)) * 2 + (o > 0 ? 1 : 0);
                        if (k2) { bn_div_small(&tg, (uint32_t)(2 * k2)); bn_div_small(&te, (uint32_t)(2 * k2)); }
                        shG = (long)k2 * k2 + (long)k2 * (o + 2 * ev);
                        bn_addsh(&G2[id2], &tg, shG);
                        bn_addsh(&E2[id2], &te, shG - k2);
                    }
                    bn_free(&G3[id]); bn_free(&E3[id]);
                }
        free(G3); free(E3); G3 = NULL; E3 = NULL;

        /* add fixed points */
        for (t = 0; t <= L; t++)
            for (C = 0; C < DC; C++)
                for (f = 0; f < 2; f++) {
                    int id2 = (t * DC + C) * 2 + f, k1;
                    if (G2[id2].len == 0) continue;
                    bn_copy(&tg, &G2[id2]);
                    bn_copy(&te, &E2[id2]);
                    for (k1 = 0; t + k1 <= L; k1++) {
                        long shG, shE;
                        if (k1) { bn_div_small(&tg, (uint32_t)k1); bn_div_small(&te, (uint32_t)k1); }
                        shG = (long)k1 * (k1 - 1) / 2 + (long)k1 * C;
                        shE = shG - k1 + ((f || k1) ? 1 : 0);
                        bn_addsh(&Gm[t + k1], &tg, shG);
                        bn_addsh(&Em[t + k1], &te, shE);
                    }
                    bn_free(&G2[id2]); bn_free(&E2[id2]);
                }
        free(G2); free(E2);
        bn_free(&tg); bn_free(&te);
        for (s = 0; s <= L; s++)
            for (k = 2; k <= L; k++) {
                bn_div_small(&Gm[s], (uint32_t)k);
                bn_div_small(&Em[s], (uint32_t)k);
            }
    }
    bn_set_u(&Am[1], 1);
    for (k = 2; k <= M; k++) {
        bn_t tmp = {NULL, 0, 0};
        int j;
        bn_copy(&Am[k], &Gm[k - 1]);
        for (j = 1; j < k; j++) {
            bn_mul(&tmp, &Am[j], &Em[k - j]);
            bn_sub(&Am[k], &tmp);
        }
        bn_free(&tmp);
    }
}

static seq_t bn_to_seq(const bn_t *x) {
    seq_t r = 0;
    int i, sh;
    for (i = x->len - 1; i >= 0; i--)
        for (sh = 24; sh >= 0; sh -= 8)
            r = r * 256 + (seq_t)(idx_t)((x->d[i] >> sh) & 255u);
    return r;
}

seq_t a(idx_t n) {
    if (n > done) {
        build((int)n);
        done = n;
    }
    return bn_to_seq(&Am[n]);
}
