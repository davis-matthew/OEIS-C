/*
 * A006526  Egyptian fraction for 1/e.
 * https://oeis.org/A006526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   greedy Egyptian fraction: a(n) = ceil(1/r(n)), r(0) = 1/e,
 *            r(n+1) = r(n) - 1/a(n). 1/e is enclosed between the partial sums
 *            D(N)/N! and D(N+1)/(N+1)! of sum (-1)^k/k!. The greedy steps run
 *            on both bounds, and N is doubled until both bounds give the same
 *            terms
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef struct { uint32_t *d; size_t n, cap; } bn;

static void bn_reserve(bn *a, size_t c)
{
    if (c > a->cap) {
        a->d = (uint32_t *)realloc(a->d, c * sizeof(uint32_t));
        a->cap = c;
    }
}

static void bn_trim(bn *a)
{
    while (a->n && a->d[a->n - 1] == 0) a->n--;
}

static void bn_set(bn *a, uint32_t v)
{
    bn_reserve(a, 1);
    a->d[0] = v;
    a->n = v ? 1 : 0;
}

static void bn_copy(bn *dst, const bn *src)
{
    bn_reserve(dst, src->n ? src->n : 1);
    if (src->n) memcpy(dst->d, src->d, src->n * sizeof(uint32_t));
    dst->n = src->n;
}

static void bn_mul_small(bn *a, uint32_t m)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < a->n; i++) {
        c += (uint64_t)a->d[i] * m;
        a->d[i] = (uint32_t)c;
        c >>= 32;
    }
    if (c) {
        bn_reserve(a, a->n + 1);
        a->d[a->n++] = (uint32_t)c;
    }
    bn_trim(a);
}

static void bn_add_small(bn *a, uint32_t v)
{
    uint64_t c = v;
    size_t i;
    for (i = 0; i < a->n && c; i++) {
        c += a->d[i];
        a->d[i] = (uint32_t)c;
        c >>= 32;
    }
    if (c) {
        bn_reserve(a, a->n + 1);
        a->d[a->n++] = (uint32_t)c;
    }
}

static void bn_sub_small(bn *a, uint32_t v)
{
    uint64_t b = v;
    size_t i;
    for (i = 0; i < a->n && b; i++) {
        uint64_t x = a->d[i];
        if (x >= b) { a->d[i] = (uint32_t)(x - b); b = 0; }
        else { a->d[i] = (uint32_t)((x + ((uint64_t)1 << 32)) - b); b = 1; }
    }
    bn_trim(a);
}

static int bn_cmp(const bn *a, const bn *b)
{
    size_t i;
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (i = a->n; i-- > 0;) {
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    }
    return 0;
}

/* a -= b, requires a >= b */
static void bn_sub(bn *a, const bn *b)
{
    int64_t br = 0;
    size_t i;
    for (i = 0; i < a->n; i++) {
        int64_t t = (int64_t)a->d[i] - br - (i < b->n ? (int64_t)b->d[i] : 0);
        if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
        a->d[i] = (uint32_t)t;
    }
    bn_trim(a);
}

/* r = a * b, r distinct from a and b */
static void bn_mul(bn *r, const bn *a, const bn *b)
{
    size_t i, j, len = a->n + b->n;
    if (a->n == 0 || b->n == 0) { r->n = 0; return; }
    bn_reserve(r, len);
    memset(r->d, 0, len * sizeof(uint32_t));
    for (i = 0; i < a->n; i++) {
        uint64_t c = 0, ai = a->d[i];
        for (j = 0; j < b->n; j++) {
            c += ai * b->d[j] + r->d[i + j];
            r->d[i + j] = (uint32_t)c;
            c >>= 32;
        }
        r->d[i + b->n] = (uint32_t)c;
    }
    r->n = len;
    bn_trim(r);
}

/* qt = ceil(u / v), u > 0, v > 0 */
static void bn_divceil(bn *qt, const bn *u, const bn *v)
{
    size_t m = u->n, n = v->n, i;
    int nz = 0;
    if (bn_cmp(u, v) <= 0) { bn_set(qt, 1); return; }
    bn_reserve(qt, m - n + 1);
    qt->n = m - n + 1;
    if (n == 1) {
        uint64_t r = 0, v0 = v->d[0];
        for (i = m; i-- > 0;) {
            uint64_t cur = (r << 32) | u->d[i];
            if (i < qt->n) qt->d[i] = (uint32_t)(cur / v0);
            r = cur % v0;
        }
        nz = r != 0;
    } else {
        const uint64_t B = (uint64_t)1 << 32;
        uint32_t *vn = (uint32_t *)malloc(n * sizeof(uint32_t));
        uint32_t *un = (uint32_t *)malloc((m + 1) * sizeof(uint32_t));
        int s = 0;
        size_t jj;
        uint32_t top = v->d[n - 1];
        while (!(top & 0x80000000u)) { top <<= 1; s++; }
        for (i = n - 1; i > 0; i--)
            vn[i] = (v->d[i] << s) | (s ? (v->d[i - 1] >> (32 - s)) : 0);
        vn[0] = v->d[0] << s;
        un[m] = s ? (u->d[m - 1] >> (32 - s)) : 0;
        for (i = m - 1; i > 0; i--)
            un[i] = (u->d[i] << s) | (s ? (u->d[i - 1] >> (32 - s)) : 0);
        un[0] = u->d[0] << s;
        for (jj = m - n + 1; jj-- > 0;) {
            size_t j = jj;
            uint64_t num = ((uint64_t)un[j + n] << 32) | un[j + n - 1];
            uint64_t qhat = num / vn[n - 1];
            uint64_t rhat = num % vn[n - 1];
            int64_t k = 0, t;
            while (qhat >= B || qhat * vn[n - 2] > ((rhat << 32) | un[j + n - 2])) {
                qhat--;
                rhat += vn[n - 1];
                if (rhat >= B) break;
            }
            for (i = 0; i < n; i++) {
                uint64_t p = qhat * vn[i];
                t = (int64_t)un[i + j] - k - (int64_t)(p & 0xFFFFFFFFu);
                un[i + j] = (uint32_t)t;
                k = (int64_t)(p >> 32) - (t >> 32);
            }
            t = (int64_t)un[j + n] - k;
            un[j + n] = (uint32_t)t;
            if (t < 0) {
                uint64_t c = 0;
                qhat--;
                for (i = 0; i < n; i++) {
                    c += (uint64_t)un[i + j] + vn[i];
                    un[i + j] = (uint32_t)c;
                    c >>= 32;
                }
                un[j + n] = (uint32_t)(un[j + n] + c);
            }
            qt->d[j] = (uint32_t)qhat;
        }
        for (i = 0; i < n; i++) if (un[i]) { nz = 1; break; }
        free(vn);
        free(un);
    }
    bn_trim(qt);
    if (nz) bn_add_small(qt, 1);
}

static bn *terms = NULL;
static idx_t nterms = 0;

/* try to compute terms 0..n with the series truncated at N; returns 1 on success */
static int try_compute(idx_t n, uint32_t N)
{
    bn p[2], q[2], d[2], t;
    uint32_t k;
    idx_t i;
    int j, ok = 1;
    memset(p, 0, sizeof p); memset(q, 0, sizeof q);
    memset(d, 0, sizeof d); memset(&t, 0, sizeof t);

    bn_set(&p[0], 1);
    bn_set(&q[0], 1);
    for (k = 1; k <= N; k++) {
        bn_mul_small(&p[0], k);
        if (k & 1) bn_sub_small(&p[0], 1); else bn_add_small(&p[0], 1);
        bn_mul_small(&q[0], k);
    }
    bn_copy(&p[1], &p[0]);
    bn_copy(&q[1], &q[0]);
    bn_mul_small(&p[1], N + 1);
    if ((N + 1) & 1) bn_sub_small(&p[1], 1); else bn_add_small(&p[1], 1);
    bn_mul_small(&q[1], N + 1);

    for (i = 0; i <= n && ok; i++) {
        for (j = 0; j < 2; j++) {
            if (p[j].n == 0) { ok = 0; break; }
            bn_divceil(&d[j], &q[j], &p[j]);
        }
        if (!ok || bn_cmp(&d[0], &d[1]) != 0) { ok = 0; break; }
        if (i >= nterms) {
            terms = (bn *)realloc(terms, (size_t)(i + 1) * sizeof(bn));
            memset(&terms[i], 0, sizeof(bn));
            bn_copy(&terms[i], &d[0]);
            nterms = i + 1;
        }
        if (i < n) {
            for (j = 0; j < 2; j++) {
                bn_mul(&t, &p[j], &d[0]);
                if (bn_cmp(&t, &q[j]) <= 0) { ok = 0; break; }
                bn_sub(&t, &q[j]);
                bn_copy(&p[j], &t);
                bn_mul(&t, &q[j], &d[0]);
                bn_copy(&q[j], &t);
            }
        }
    }
    for (j = 0; j < 2; j++) { free(p[j].d); free(q[j].d); free(d[j].d); }
    free(t.d);
    return ok;
}

seq_t a(idx_t n)
{
    seq_t r = 0;
    size_t i;
    if (n >= nterms) {
        uint32_t N = 32;
        while (!try_compute(n, N)) N *= 2;
    }
    for (i = terms[n].n; i-- > 0;) {
        r = r * 65536;
        r = r * 65536;
        r = r + (idx_t)terms[n].d[i];
    }
    return r;
}
