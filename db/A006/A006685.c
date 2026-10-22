/*
 * A006685  Coefficients for numerical integration.
 * https://oeis.org/A006685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002198(n)/(2n+1), where A002198(n) = denominator of
 *            c(n+1), c(m) = [d^(2m)] d/(2*arcsinh(d/2)) = (1/(2m)!) *
 *            Integral_{s=-1/2..1/2} Prod_{j=0..m-1} (s^2 - j^2) ds =
 *            (1/(2m)!) * Sum_{i=1..m} p_i / (4^i*(2i+1)), Sum_i p_i u^i =
 *            u*Prod_{j=1..m-1} (u - j^2). Computed exactly with internal
 *            mul...
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n; size_t cap; } bn_t;

static void bn_init(bn_t *x) { x->d = NULL; x->n = 0; x->cap = 0; }
static void bn_free(bn_t *x) { free(x->d); bn_init(x); }
static void bn_reserve(bn_t *x, size_t c) {
    if (c > x->cap) {
        size_t nc = c * 2 + 4;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}
static void bn_set(bn_t *x, uint32_t v) { bn_reserve(x, 1); x->d[0] = v; x->n = v ? 1 : 0; }
static void bn_trim(bn_t *x) { while (x->n && x->d[x->n - 1] == 0) x->n--; }
static void bn_copy(bn_t *dst, const bn_t *src) {
    bn_reserve(dst, src->n + 1);
    if (src->n) memcpy(dst->d, src->d, src->n * sizeof(uint32_t));
    dst->n = src->n;
}
static void bn_mul_small(bn_t *x, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    if (m == 0) { x->n = 0; return; }
    for (i = 0; i < x->n; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) { bn_reserve(x, x->n + 1); x->d[x->n++] = (uint32_t)c; }
}
static void bn_add(bn_t *x, const bn_t *y) {
    size_t i, xn = x->n, n = x->n > y->n ? x->n : y->n;
    uint64_t c = 0;
    bn_reserve(x, n + 1);
    for (i = 0; i < n; i++) {
        uint64_t t = c + (i < xn ? x->d[i] : 0) + (i < y->n ? y->d[i] : 0);
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    x->n = n;
    if (c) x->d[x->n++] = (uint32_t)c;
}
static int bn_cmp(const bn_t *x, const bn_t *y) {
    size_t i;
    if (x->n != y->n) return x->n < y->n ? -1 : 1;
    for (i = x->n; i-- > 0;)
        if (x->d[i] != y->d[i]) return x->d[i] < y->d[i] ? -1 : 1;
    return 0;
}
static void bn_sub(bn_t *x, const bn_t *y) { /* x -= y, x >= y */
    size_t i;
    int64_t b = 0;
    for (i = 0; i < x->n; i++) {
        int64_t t = (int64_t)x->d[i] - (i < y->n ? (int64_t)y->d[i] : 0) - b;
        if (t < 0) { t += ((int64_t)1 << 32); b = 1; } else b = 0;
        x->d[i] = (uint32_t)t;
    }
    bn_trim(x);
}
static uint32_t bn_div_small(bn_t *x, uint32_t m) {
    uint64_t r = 0;
    size_t i;
    for (i = x->n; i-- > 0;) {
        uint64_t t = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(t / m);
        r = t % m;
    }
    bn_trim(x);
    return (uint32_t)r;
}
static uint32_t bn_mod_small(const bn_t *x, uint32_t m) {
    uint64_t r = 0;
    size_t i;
    for (i = x->n; i-- > 0;) r = ((r << 32) | x->d[i]) % m;
    return (uint32_t)r;
}
static void bn_mul(bn_t *r, const bn_t *x, const bn_t *y) {
    size_t i, j;
    if (!x->n || !y->n) { r->n = 0; return; }
    bn_reserve(r, x->n + y->n);
    memset(r->d, 0, (x->n + y->n) * sizeof(uint32_t));
    for (i = 0; i < x->n; i++) {
        uint64_t c = 0, xi = x->d[i];
        for (j = 0; j < y->n; j++) {
            uint64_t t = xi * y->d[j] + r->d[i + j] + c;
            r->d[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        r->d[i + y->n] = (uint32_t)c;
    }
    r->n = x->n + y->n;
    bn_trim(r);
}

/* memo: magnitudes of coefficients of Prod_{j=1..QJ} (u - j^2) */
static bn_t *Qc = NULL;
static size_t Qcap = 0;
static uint64_t QJ = 0;
static int Qinit = 0;

static void q_ensure(uint64_t J) {
    uint64_t j, i;
    if (Qcap < J + 2) {
        size_t nc = (size_t)(J + 2) * 2, s;
        Qc = (bn_t *)realloc(Qc, nc * sizeof(bn_t));
        for (s = Qcap; s < nc; s++) bn_init(&Qc[s]);
        Qcap = nc;
    }
    if (!Qinit || QJ > J) {
        bn_set(&Qc[0], 1);
        QJ = 0;
        Qinit = 1;
    }
    while (QJ < J) {
        j = QJ + 1;
        bn_copy(&Qc[j], &Qc[j - 1]);
        for (i = j - 1; i >= 1; i--) {
            bn_mul_small(&Qc[i], (uint32_t)j);
            bn_mul_small(&Qc[i], (uint32_t)j);
            bn_add(&Qc[i], &Qc[i - 1]);
        }
        bn_mul_small(&Qc[0], (uint32_t)j);
        bn_mul_small(&Qc[0], (uint32_t)j);
        QJ = j;
    }
}

seq_t a(idx_t n) {
    uint64_t k = (uint64_t)n + 1, top = 2 * k + 1, i, p;
    char *comp;
    bn_t L, pos, neg, t, prod, N;
    seq_t r = 1;

    q_ensure(k - 1);

    comp = (char *)calloc((size_t)top + 1, 1);
    for (p = 2; p * p <= top; p++)
        if (!comp[p])
            for (i = p * p; i <= top; i += p) comp[i] = 1;

    bn_init(&L); bn_init(&pos); bn_init(&neg); bn_init(&t); bn_init(&prod); bn_init(&N);
    bn_set(&L, 1);
    for (p = 3; p <= top; p += 2) {
        if (comp[p]) continue;
        {
            uint64_t pe = p;
            while (pe * p <= top) pe *= p;
            bn_mul_small(&L, (uint32_t)pe);
        }
    }
    bn_set(&pos, 0);
    bn_set(&neg, 0);
    for (i = 1; i <= k; i++) {
        bn_mul_small(&pos, 4);
        bn_mul_small(&neg, 4);
        bn_copy(&t, &L);
        bn_div_small(&t, (uint32_t)(2 * i + 1));
        bn_mul(&prod, &t, &Qc[i - 1]);
        if ((k - i) % 2 == 0) bn_add(&pos, &prod);
        else bn_add(&neg, &prod);
    }
    if (bn_cmp(&pos, &neg) >= 0) { bn_copy(&N, &pos); bn_sub(&N, &neg); }
    else { bn_copy(&N, &neg); bn_sub(&N, &pos); }

    for (p = 2; p <= top; p++) {
        uint64_t eM = 0, eN = 0, eD = 0, q, m;
        long long ex, e;
        if (comp[p]) continue;
        for (q = p; q <= 2 * k; q *= p) eM += (2 * k) / q;
        if (p == 2) eM += 2 * k;
        else for (q = p; q <= top; q *= p) eM++;
        bn_copy(&t, &N);
        while (eN < eM && t.n && bn_mod_small(&t, (uint32_t)p) == 0) {
            bn_div_small(&t, (uint32_t)p);
            eN++;
        }
        m = 2 * k - 1;
        while (m % p == 0) { m /= p; eD++; }
        ex = (long long)eM - (long long)eN - (long long)eD;
        for (e = 0; e < ex; e++) r *= (seq_t)p;
    }

    bn_free(&L); bn_free(&pos); bn_free(&neg); bn_free(&t); bn_free(&prod); bn_free(&N);
    free(comp);
    return r;
}
