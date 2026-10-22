/*
 * A007125  Number of connected strength-1 Eulerian graphs with n nodes, 2 of
 *          odd degree.
 * https://oeis.org/A007125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside/Polya count of unlabeled graphs by number of odd
 *            vertices. For a vertex permutation with N edge orbits, E even
 *            cycles and O odd cycles: the fixed graphs with a given
 *            odd-vertex pattern number 2^(N-E-max(O-1,0)), and the possible
 *            patterns are those with an even number of odd-length cycles...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 11, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   gcd_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_ll.h"

typedef struct { uint32_t *d; int len, cap; } bn_t;

static void bn_ext(bn_t *a, int n) {
    if (n > a->cap) {
        int nc = n * 2 + 4;
        a->d = (uint32_t *)realloc(a->d, (size_t)nc * sizeof(uint32_t));
        a->cap = nc;
    }
    if (n > a->len) {
        memset(a->d + a->len, 0, (size_t)(n - a->len) * sizeof(uint32_t));
        a->len = n;
    }
}
static void bn_trim(bn_t *a) { while (a->len > 0 && a->d[a->len - 1] == 0) a->len--; }
static void bn_setu(bn_t *a, uint32_t v) { a->len = 0; bn_ext(a, 1); a->d[0] = v; bn_trim(a); }
static void bn_copy(bn_t *a, const bn_t *b) {
    a->len = 0;
    bn_ext(a, b->len);
    if (b->len) memcpy(a->d, b->d, (size_t)b->len * sizeof(uint32_t));
}
static void bn_mulsmall(bn_t *a, uint32_t q) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < a->len; i++) {
        uint64_t v = (uint64_t)a->d[i] * q + c;
        a->d[i] = (uint32_t)v; c = v >> 32;
    }
    if (c) { int l = a->len; bn_ext(a, l + 1); a->d[l] = (uint32_t)c; }
}
static void bn_divsmall(bn_t *a, uint32_t q) {
    uint64_t r = 0;
    int i;
    if (q == 1) return;
    for (i = a->len - 1; i >= 0; i--) {
        uint64_t v = (r << 32) | a->d[i];
        a->d[i] = (uint32_t)(v / q); r = v % q;
    }
    bn_trim(a);
}
/* a += (x * c) << s, with c < 2^32 */
static void bn_addshift(bn_t *a, const bn_t *x, int s, uint64_t c) {
    int ws = s >> 5, bs = s & 31, i, need;
    uint64_t cm = 0, sp = 0, ac = 0;
    if (x->len == 0 || c == 0) return;
    need = ws + x->len + 3;
    if (a->len > need) need = a->len;
    bn_ext(a, need + 1);
    for (i = 0;; i++) {
        uint64_t d, w, t;
        if (i < x->len) {
            uint64_t v = (uint64_t)x->d[i] * c + cm;
            d = v & 0xffffffffULL; cm = v >> 32;
        } else {
            if (!cm && !sp && !ac) break;
            d = cm & 0xffffffffULL; cm >>= 32;
        }
        w = (d << bs) | sp; sp = w >> 32;
        t = (uint64_t)a->d[ws + i] + (w & 0xffffffffULL) + ac;
        a->d[ws + i] = (uint32_t)t; ac = t >> 32;
    }
    bn_trim(a);
}
static void bn_addmul(bn_t *acc, const bn_t *a, const bn_t *b) {
    int j;
    for (j = 0; j < a->len; j++) bn_addshift(acc, b, 32 * j, a->d[j]);
}
static void bn_sub(bn_t *a, const bn_t *b) {
    int64_t br = 0;
    int i;
    for (i = 0; i < a->len; i++) {
        int64_t t = (int64_t)a->d[i] - br - (i < b->len ? (int64_t)b->d[i] : 0);
        if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
        a->d[i] = (uint32_t)t;
    }
    bn_trim(a);
}
static seq_t bn_to_seq(const bn_t *a) {
    seq_t r = 0;
    int i, sh;
    for (i = a->len - 1; i >= 0; i--)
        for (sh = 24; sh >= 0; sh -= 8) {
            r *= 256;
            r += (int)((a->d[i] >> sh) & 255);
        }
    return r;
}

static int *stK = NULL, *stM = NULL;
static int depth = 0;
static bn_t *lv = NULL;
static int lvcap = 0;
static bn_t acc0, acc2;

static void rec(int rem, int maxk, long long N, int E, int O, int m1, int m2, int lvl) {
    int k;
    if (rem == 0) {
        long long r = E + (O > 0 ? O - 1 : 0);
        int e = (int)(N - r);
        uint64_t c = (uint64_t)m2 + (uint64_t)m1 * (uint64_t)(m1 - 1) / 2;
        bn_addshift(&acc0, &lv[lvl], e, 1);
        if (c) bn_addshift(&acc2, &lv[lvl], e, c);
        return;
    }
    for (k = (maxk < rem ? maxk : rem); k >= 1; k--) {
        long long gs = 0;
        int j, m;
        for (j = 0; j < depth; j++) gs += (long long)stM[j] * gcd_ll(k, stK[j]);
        bn_copy(&lv[lvl + 1], &lv[lvl]);
        for (m = 1; m * k <= rem; m++) {
            long long nN;
            bn_divsmall(&lv[lvl + 1], (uint32_t)k);
            bn_divsmall(&lv[lvl + 1], (uint32_t)m);
            nN = N + (long long)m * (k / 2) + (long long)k * m * (m - 1) / 2 + (long long)m * gs;
            stK[depth] = k; stM[depth] = m; depth++;
            rec(rem - m * k, k - 1, nN,
                E + (k % 2 == 0 ? m : 0), O + (k % 2 ? m : 0),
                k == 1 ? m : m1, k == 2 ? m : m2, lvl + 1);
            depth--;
        }
    }
}

static void compute(int n, bn_t *g0, bn_t *g2) {
    int i;
    if (n + 2 > lvcap) {
        int nc = n + 2;
        lv = (bn_t *)realloc(lv, (size_t)nc * sizeof(bn_t));
        memset(lv + lvcap, 0, (size_t)(nc - lvcap) * sizeof(bn_t));
        lvcap = nc;
        stK = (int *)realloc(stK, (size_t)nc * sizeof(int));
        stM = (int *)realloc(stM, (size_t)nc * sizeof(int));
    }
    bn_setu(&lv[0], 1);
    for (i = 2; i <= n; i++) bn_mulsmall(&lv[0], (uint32_t)i);
    acc0.len = 0; acc2.len = 0;
    depth = 0;
    rec(n, n, 0, 0, 0, 0, 0, 0);
    for (i = 2; i <= n; i++) { bn_divsmall(&acc0, (uint32_t)i); bn_divsmall(&acc2, (uint32_t)i); }
    bn_copy(g0, &acc0);
    bn_copy(g2, &acc2);
}

static bn_t *G0 = NULL, *G2 = NULL, *C2 = NULL;
static long long done = -1, memcap = 0;

seq_t a(idx_t n) {
    long long nn = (long long)n, i;
    if (nn >= memcap) {
        long long nc = nn + 16;
        G0 = (bn_t *)realloc(G0, (size_t)nc * sizeof(bn_t));
        G2 = (bn_t *)realloc(G2, (size_t)nc * sizeof(bn_t));
        C2 = (bn_t *)realloc(C2, (size_t)nc * sizeof(bn_t));
        memset(G0 + memcap, 0, (size_t)(nc - memcap) * sizeof(bn_t));
        memset(G2 + memcap, 0, (size_t)(nc - memcap) * sizeof(bn_t));
        memset(C2 + memcap, 0, (size_t)(nc - memcap) * sizeof(bn_t));
        memcap = nc;
    }
    for (i = done + 1; i <= nn; i++) {
        if (i == 0) {
            bn_setu(&G0[0], 1);
            G2[0].len = 0;
            C2[0].len = 0;
        } else {
            bn_t s = {NULL, 0, 0};
            long long k;
            compute((int)i, &G0[i], &G2[i]);
            for (k = 1; k < i; k++) bn_addmul(&s, &G0[k], &C2[i - k]);
            bn_copy(&C2[i], &G2[i]);
            bn_sub(&C2[i], &s);
            free(s.d);
        }
        done = i;
    }
    return bn_to_seq(&C2[nn]);
}
