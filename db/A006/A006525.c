/*
 * A006525  Denominators of greedy Egyptian fraction for e - 2.
 * https://oeis.org/A006525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = ceiling(1/(e - 2 - Sum_{j=1..n-1} 1/a(j))), computed
 *            exactly with integer bounds: e - 2 lies strictly between S/K!
 *            and (S+1)/K!, where S = Sum_{k=2..K} K!/k!. K is doubled until
 *            both bounds give the same ceiling for every term
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

typedef struct { uint32_t *d; size_t n, cap; } bn;

static bn bn_new(size_t cap) {
    bn r;
    if (cap < 1) cap = 1;
    r.d = (uint32_t *)calloc(cap, sizeof(uint32_t));
    r.n = 0; r.cap = cap;
    return r;
}
static void bn_free(bn *x) { free(x->d); x->d = NULL; x->n = x->cap = 0; }
static void bn_trim(bn *x) { while (x->n && x->d[x->n - 1] == 0) x->n--; }
static void bn_reserve(bn *x, size_t c) {
    if (c > x->cap) {
        size_t nc = c * 2;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        memset(x->d + x->cap, 0, (nc - x->cap) * sizeof(uint32_t));
        x->cap = nc;
    }
}
static bn bn_copy(const bn *x) {
    bn r = bn_new(x->n + 1);
    memcpy(r.d, x->d, x->n * sizeof(uint32_t));
    r.n = x->n;
    return r;
}
/* x = x*m + a */
static void bn_muladd_small(bn *x, uint32_t m, uint32_t a) {
    uint64_t c = a;
    size_t i;
    for (i = 0; i < x->n; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) { bn_reserve(x, x->n + 1); x->d[x->n++] = (uint32_t)c; }
}
static bn bn_mul(const bn *a, const bn *b) {
    bn r = bn_new(a->n + b->n + 1);
    size_t i, j;
    for (i = 0; i < a->n; i++) {
        uint64_t c = 0, ai = a->d[i];
        if (!ai) continue;
        for (j = 0; j < b->n; j++) {
            uint64_t t = ai * b->d[j] + r.d[i + j] + c;
            r.d[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        r.d[i + b->n] = (uint32_t)c;
    }
    r.n = a->n + b->n;
    bn_trim(&r);
    return r;
}
/* a -= b, requires a >= b */
static void bn_sub_inplace(bn *a, const bn *b) {
    int64_t br = 0;
    size_t i;
    for (i = 0; i < a->n; i++) {
        int64_t t = (int64_t)a->d[i] - br - (i < b->n ? (int64_t)b->d[i] : 0);
        if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
        a->d[i] = (uint32_t)t;
    }
    bn_trim(a);
}
static int bn_cmp(const bn *a, const bn *b) {
    size_t i;
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (i = a->n; i-- > 0;)
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    return 0;
}
/* ceiling(u / v), v > 0 */
static bn bn_ceildiv(const bn *u, const bn *v) {
    bn q;
    int nonzero = 0;
    size_t m = u->n, nv = v->n, i;
    if (bn_cmp(u, v) <= 0) {
        q = bn_new(1);
        if (u->n) { q.d[0] = 1; q.n = 1; }
        return q;
    }
    q = bn_new(m - nv + 2);
    if (nv == 1) {
        uint64_t rem = 0, v0 = v->d[0];
        for (i = m; i-- > 0;) {
            uint64_t t = (rem << 32) | u->d[i];
            q.d[i] = (uint32_t)(t / v0);
            rem = t % v0;
        }
        q.n = m;
        nonzero = rem != 0;
    } else {
        const uint64_t B = (uint64_t)1 << 32;
        int s = 0;
        uint32_t top = v->d[nv - 1];
        uint32_t *vn, *un;
        long long j;
        while (!(top & 0x80000000u)) { top <<= 1; s++; }
        vn = (uint32_t *)malloc(nv * sizeof(uint32_t));
        un = (uint32_t *)malloc((m + 1) * sizeof(uint32_t));
        for (i = nv - 1; i > 0; i--)
            vn[i] = (uint32_t)(((uint64_t)v->d[i] << s) | ((uint64_t)v->d[i - 1] >> (32 - s)));
        vn[0] = (uint32_t)((uint64_t)v->d[0] << s);
        un[m] = (uint32_t)((uint64_t)u->d[m - 1] >> (32 - s));
        for (i = m - 1; i > 0; i--)
            un[i] = (uint32_t)(((uint64_t)u->d[i] << s) | ((uint64_t)u->d[i - 1] >> (32 - s)));
        un[0] = (uint32_t)((uint64_t)u->d[0] << s);
        for (j = (long long)(m - nv); j >= 0; j--) {
            uint64_t num = ((uint64_t)un[j + nv] << 32) | un[j + nv - 1];
            uint64_t qhat = num / vn[nv - 1];
            uint64_t rhat = num - qhat * vn[nv - 1];
            int64_t k, t;
            while (qhat >= B || qhat * vn[nv - 2] > ((rhat << 32) | un[j + nv - 2])) {
                qhat--;
                rhat += vn[nv - 1];
                if (rhat >= B) break;
            }
            k = 0;
            for (i = 0; i < nv; i++) {
                uint64_t p = qhat * vn[i];
                t = (int64_t)un[i + j] - k - (int64_t)(p & 0xFFFFFFFFu);
                un[i + j] = (uint32_t)t;
                k = (int64_t)(p >> 32) - (t >> 32);
            }
            t = (int64_t)un[j + nv] - k;
            un[j + nv] = (uint32_t)t;
            if (t < 0) {
                uint64_t c = 0;
                qhat--;
                for (i = 0; i < nv; i++) {
                    uint64_t tt = (uint64_t)un[i + j] + vn[i] + c;
                    un[i + j] = (uint32_t)tt;
                    c = tt >> 32;
                }
                un[j + nv] = (uint32_t)(un[j + nv] + c);
            }
            q.d[j] = (uint32_t)qhat;
        }
        for (i = 0; i < nv; i++) if (un[i]) { nonzero = 1; break; }
        q.n = m - nv + 1;
        free(vn);
        free(un);
    }
    bn_trim(&q);
    if (nonzero) bn_muladd_small(&q, 1, 1);
    return q;
}

static bn *terms = NULL;
static idx_t nterms = 0;
static unsigned long Kcur = 16;

static int compute_terms(idx_t n, unsigned long K, bn *out) {
    bn D = bn_new(4), Plo = bn_new(4), Phi, Q;
    unsigned long k;
    idx_t i, got = 0;
    int ok = 1;
    D.d[0] = 2; D.n = 1;
    Plo.d[0] = 1; Plo.n = 1;
    for (k = 3; k <= K; k++) {
        bn_muladd_small(&D, (uint32_t)k, 0);
        bn_muladd_small(&Plo, (uint32_t)k, 1);
    }
    Phi = bn_copy(&Plo);
    bn_muladd_small(&Phi, 1, 1);
    Q = D;
    for (i = 0; i < n; i++) {
        bn alo, ahi, t;
        if (Plo.n == 0) { ok = 0; break; }
        alo = bn_ceildiv(&Q, &Plo);
        ahi = bn_ceildiv(&Q, &Phi);
        if (bn_cmp(&alo, &ahi) != 0) { bn_free(&alo); bn_free(&ahi); ok = 0; break; }
        bn_free(&ahi);
        out[i] = alo;
        got++;
        t = bn_mul(&Plo, &alo); bn_sub_inplace(&t, &Q); bn_free(&Plo); Plo = t;
        t = bn_mul(&Phi, &alo); bn_sub_inplace(&t, &Q); bn_free(&Phi); Phi = t;
        t = bn_mul(&Q, &alo); bn_free(&Q); Q = t;
    }
    bn_free(&Plo); bn_free(&Phi); bn_free(&Q);
    if (!ok) {
        for (i = 0; i < got; i++) bn_free(&out[i]);
    }
    return ok;
}

static seq_t bn_to_seq(const bn *x) {
    seq_t r = 0;
    size_t i;
    if (seq_is_fixed_width()) {
        size_t top = x->n < 4 ? x->n : 4;
        for (i = top; i-- > 0;) {
            idx_t w = x->d[i];
            r <<= 8; r <<= 8; r <<= 8; r <<= 8;
            r += w;
        }
    } else {
        for (i = x->n; i-- > 0;) {
            idx_t w = x->d[i];
            r <<= 32;
            r += w;
        }
    }
    return r;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > nterms) {
        bn *out = (bn *)malloc(n * sizeof(bn));
        idx_t i;
        while (!compute_terms(n, Kcur, out)) Kcur *= 2;
        for (i = 0; i < nterms; i++) bn_free(&terms[i]);
        free(terms);
        terms = out;
        nterms = n;
    }
    return bn_to_seq(&terms[n - 1]);
}
