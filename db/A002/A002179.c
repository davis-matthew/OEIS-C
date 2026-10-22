/*
 * A002179  Numerators of Cotesian numbers (not in lowest terms):
 *          A002176*C(n,2).
 * https://oeis.org/A002179
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = A002176(n-1) * C(n-1,2), where C(m,k) = (1/m) *
 *            Integral_{t=0..m} Product_{j=0..m, j<>k} (t-j)/(k-j) dt is the
 *            Cotes number (Newton-Cotes weight on m intervals), and
 *            A002176(m) is the least common denominator of C(m,0..m).
 *            Computed exactly with internal big integers: with T =
 *            m*m!*lcm(1..m...
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 13, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   c2179_vfact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/c2179_vfact.h"

typedef struct { uint32_t *d; size_t len; } c2179_bn;

static void c2179_norm(c2179_bn *a) { while (a->len && a->d[a->len - 1] == 0) a->len--; }

static void c2179_set(c2179_bn *a, uint64_t v) {
    a->len = 0;
    while (v) { a->d[a->len++] = (uint32_t)v; v >>= 32; }
}

static void c2179_copy(c2179_bn *dst, const c2179_bn *src) {
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
}

static void c2179_add(c2179_bn *a, const c2179_bn *b) {
    size_t n = a->len > b->len ? a->len : b->len, i;
    uint64_t c = 0;
    for (i = 0; i < n; i++) {
        uint64_t s = c;
        if (i < a->len) s += a->d[i];
        if (i < b->len) s += b->d[i];
        a->d[i] = (uint32_t)s;
        c = s >> 32;
    }
    if (c) a->d[n++] = (uint32_t)c;
    a->len = n;
}

/* a -= b, requires a >= b */
static void c2179_sub(c2179_bn *a, const c2179_bn *b) {
    int64_t br = 0;
    size_t i;
    for (i = 0; i < a->len; i++) {
        int64_t s = (int64_t)a->d[i] - br - (i < b->len ? (int64_t)b->d[i] : 0);
        if (s < 0) { s += ((int64_t)1 << 32); br = 1; } else br = 0;
        a->d[i] = (uint32_t)s;
    }
    c2179_norm(a);
}

static int c2179_cmp(const c2179_bn *a, const c2179_bn *b) {
    size_t i;
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    i = a->len;
    while (i--) if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    return 0;
}

static void c2179_muls(c2179_bn *a, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    if (m == 0) { a->len = 0; return; }
    for (i = 0; i < a->len; i++) {
        uint64_t t = (uint64_t)a->d[i] * m + c;
        a->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) a->d[a->len++] = (uint32_t)c;
}

/* acc += x * y */
static void c2179_addmul(c2179_bn *acc, const c2179_bn *x, const c2179_bn *y) {
    size_t i, j, top;
    if (!x->len || !y->len) return;
    top = x->len + y->len + 1;
    if (acc->len + 1 > top) top = acc->len + 1;
    for (i = acc->len; i < top; i++) acc->d[i] = 0;
    for (i = 0; i < x->len; i++) {
        uint64_t c = 0, xi = x->d[i];
        size_t k;
        if (!xi) continue;
        for (j = 0; j < y->len; j++) {
            uint64_t t = xi * y->d[j] + acc->d[i + j] + c;
            acc->d[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        k = i + y->len;
        while (c) {
            uint64_t t = (uint64_t)acc->d[k] + c;
            acc->d[k] = (uint32_t)t;
            c = t >> 32;
            k++;
        }
    }
    acc->len = top;
    c2179_norm(acc);
}

static uint32_t c2179_divs(c2179_bn *a, uint32_t d) {
    uint64_t r = 0;
    size_t i = a->len;
    while (i--) {
        uint64_t cur = (r << 32) | a->d[i];
        a->d[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
    c2179_norm(a);
    return (uint32_t)r;
}

static uint32_t c2179_mods(const c2179_bn *a, uint32_t d) {
    uint64_t r = 0;
    size_t i = a->len;
    while (i--) r = ((r << 32) | a->d[i]) % d;
    return (uint32_t)r;
}

seq_t a(idx_t idx) {
    uint64_t n, N1, i, j, k, p;
    int bl = 0, sign;
    size_t bits, W, nb, t;
    uint32_t *pool;
    c2179_bn *B, *c, *w, *R, *N, *Lb, *q, *pw, *pos, *neg, *scr, *res;
    unsigned char *Rneg, *Nneg, *comp;
    seq_t r;

    if (idx < 3) return 0;
    n = idx - 1;
    N1 = n + 1;
    { uint64_t x = N1; while (x) { bl++; x >>= 1; } }
    bits = (size_t)((3 * n + 10) * (uint64_t)bl + 2 * n + 200);
    W = bits / 32 + 8;
    nb = (size_t)(4 * n + 16);
    pool = (uint32_t *)calloc(nb * W, sizeof(uint32_t));
    B = (c2179_bn *)calloc(nb, sizeof(c2179_bn));
    for (t = 0; t < nb; t++) { B[t].d = pool + t * W; B[t].len = 0; }
    c = B;
    w = B + (n + 2);
    R = w + (n + 1);
    N = R + (n + 1);
    Lb = N + (n + 1);
    q = Lb + 1; pw = Lb + 2; pos = Lb + 3; neg = Lb + 4; scr = Lb + 5; res = Lb + 6;
    Rneg = (unsigned char *)calloc(n + 1, 1);
    Nneg = (unsigned char *)calloc(n + 1, 1);
    comp = (unsigned char *)calloc(N1 + 1, 1);

    for (i = 2; i <= N1; i++)
        if (!comp[i]) for (j = i * i; j <= N1; j += i) comp[j] = 1;

    /* L = lcm(1..n+1) */
    c2179_set(Lb, 1);
    for (p = 2; p <= N1; p++) {
        uint64_t pe;
        if (comp[p]) continue;
        pe = p;
        while (pe * p <= N1) pe *= p;
        c2179_muls(Lb, (uint32_t)pe);
    }

    /* w_m = n^(m+1) * L/(m+1) */
    c2179_set(pw, n);
    for (i = 0; i <= n; i++) {
        c2179_copy(q, Lb);
        c2179_divs(q, (uint32_t)(i + 1));
        w[i].len = 0;
        c2179_addmul(&w[i], q, pw);
        c2179_muls(pw, (uint32_t)n);
    }

    /* |p_i|: coefficients of Product_{j=0..n} (t + j); sign of p_i is (-1)^(n+1-i) */
    c2179_set(&c[0], 1);
    for (j = 0; j <= n; j++) {
        for (i = j + 1; i >= 1; i--) {
            c2179_muls(&c[i], (uint32_t)j);
            c2179_add(&c[i], &c[i - 1]);
        }
        c2179_muls(&c[0], (uint32_t)j);
    }

    /* R_j = sum_{i=j+1..n+1} p_i w_{i-1-j} */
    for (j = 0; j <= n; j++) {
        pos->len = 0; neg->len = 0;
        for (i = j + 1; i <= N1; i++) {
            if (((N1 - i) & 1) == 0) c2179_addmul(pos, &c[i], &w[i - 1 - j]);
            else c2179_addmul(neg, &c[i], &w[i - 1 - j]);
        }
        if (c2179_cmp(pos, neg) >= 0) { c2179_copy(&R[j], pos); c2179_sub(&R[j], neg); Rneg[j] = 0; }
        else { c2179_copy(&R[j], neg); c2179_sub(&R[j], pos); Rneg[j] = 1; }
    }

    /* N_k = sum_j R_j k^j  (= L * Integral_0^n P(t)/(t-k) dt) */
    for (k = 0; k <= n; k++) {
        pos->len = 0; neg->len = 0;
        for (j = n + 1; j-- > 0;) {
            c2179_muls(pos, (uint32_t)k);
            c2179_muls(neg, (uint32_t)k);
            if (R[j].len) {
                if (Rneg[j]) c2179_add(neg, &R[j]); else c2179_add(pos, &R[j]);
            }
        }
        if (c2179_cmp(pos, neg) >= 0) { c2179_copy(&N[k], pos); c2179_sub(&N[k], neg); Nneg[k] = 0; }
        else { c2179_copy(&N[k], neg); c2179_sub(&N[k], pos); Nneg[k] = 1; }
    }

    /* M_2 magnitude = binom(n,2) * |N_2|, sign (-1)^n * sign(N_2) */
    c2179_copy(res, &N[2]);
    c2179_muls(res, (uint32_t)n);
    c2179_muls(res, (uint32_t)(n - 1));
    c2179_divs(res, 2);
    sign = (Nneg[2] ^ (int)(n & 1)) ? 1 : 0;

    /* divide by gcd(T, M_0..M_n), prime by prime */
    for (p = 2; p <= N1; p++) {
        uint64_t vT, vnf, gmin, pe, e;
        if (comp[p]) continue;
        vnf = c2179_vfact(n, p);
        vT = vnf;
        { uint64_t x = n; while (x % p == 0) { vT++; x /= p; } }
        pe = p; e = 1;
        while (pe * p <= N1) { pe *= p; e++; }
        vT += e;
        gmin = vT;
        for (k = 0; k <= n && gmin > 0; k++) {
            uint64_t vb, need, v = 0;
            if (N[k].len == 0) continue;
            vb = vnf - c2179_vfact(k, p) - c2179_vfact(n - k, p);
            if (vb >= gmin) continue;
            need = gmin - vb;
            if (c2179_mods(&N[k], (uint32_t)p) == 0) {
                c2179_copy(scr, &N[k]);
                while (v < need && c2179_mods(scr, (uint32_t)p) == 0) {
                    c2179_divs(scr, (uint32_t)p);
                    v++;
                }
            }
            if (vb + v < gmin) gmin = vb + v;
        }
        for (e = 0; e < gmin; e++) c2179_divs(res, (uint32_t)p);
    }

    r = 0;
    t = res->len;
    while (t--) {
        r = r * 65536 + (seq_t)(long long)(res->d[t] >> 16);
        r = r * 65536 + (seq_t)(long long)(res->d[t] & 0xFFFFu);
    }
    if (sign) r = -r;

    free(comp); free(Nneg); free(Rneg); free(B); free(pool);
    return r;
}
