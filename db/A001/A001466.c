/*
 * A001466  Denominators of greedy Egyptian fraction expansion of Pi - 3.
 * https://oeis.org/A001466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   greedy (Sylvester) expansion: x_1 = Pi - 3, a(n) = ceil(1/x_n),
 *            x_{n+1} = x_n - 1/a(n). Pi is computed exactly to P bits with
 *            Machin's formula, Pi = 16*arctan(1/5) - 4*arctan(1/239), giving
 *            rational lower and upper bounds for Pi - 3. The greedy expansion
 *            runs on both bounds, and only terms on whi...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct { uint32_t *w; size_t n; } g66_bn;

static g66_bn g66_alloc(size_t n) {
    g66_bn r;
    r.n = n;
    r.w = (uint32_t *)calloc(n ? n : 1, sizeof(uint32_t));
    return r;
}
static void g66_trim(g66_bn *a) { while (a->n && a->w[a->n - 1] == 0) a->n--; }
static void g66_free(g66_bn *a) { free(a->w); a->w = NULL; a->n = 0; }
static g66_bn g66_copy(const g66_bn *a) {
    g66_bn r = g66_alloc(a->n);
    if (a->n) memcpy(r.w, a->w, a->n * sizeof(uint32_t));
    return r;
}
static g66_bn g66_pow2(size_t e) {
    g66_bn r = g66_alloc(e / 32 + 1);
    r.w[e / 32] = (uint32_t)1 << (e % 32);
    return r;
}
static void g66_grow(g66_bn *a, size_t m) {
    a->w = (uint32_t *)realloc(a->w, m * sizeof(uint32_t));
    for (size_t i = a->n; i < m; i++) a->w[i] = 0;
}
static int g66_cmp(const g66_bn *a, const g66_bn *b) {
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (size_t i = a->n; i-- > 0;)
        if (a->w[i] != b->w[i]) return a->w[i] < b->w[i] ? -1 : 1;
    return 0;
}
static uint32_t g66_divs(g66_bn *a, uint32_t d) {
    uint64_t rem = 0;
    for (size_t i = a->n; i-- > 0;) {
        uint64_t cur = (rem << 32) | a->w[i];
        a->w[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    g66_trim(a);
    return (uint32_t)rem;
}
static void g66_muls(g66_bn *a, uint32_t m) {
    g66_grow(a, a->n + 1);
    uint64_t c = 0;
    for (size_t i = 0; i < a->n; i++) {
        uint64_t cur = (uint64_t)a->w[i] * m + c;
        a->w[i] = (uint32_t)cur;
        c = cur >> 32;
    }
    a->w[a->n] = (uint32_t)c;
    a->n++;
    g66_trim(a);
}
static void g66_addto(g66_bn *a, const g66_bn *b) {
    size_t m = (a->n > b->n ? a->n : b->n) + 1;
    g66_grow(a, m);
    uint64_t c = 0;
    for (size_t i = 0; i < m; i++) {
        uint64_t cur = (uint64_t)a->w[i] + (i < b->n ? b->w[i] : 0) + c;
        a->w[i] = (uint32_t)cur;
        c = cur >> 32;
    }
    a->n = m;
    g66_trim(a);
}
static void g66_subfrom(g66_bn *a, const g66_bn *b) { /* a >= b */
    int64_t br = 0;
    for (size_t i = 0; i < a->n; i++) {
        int64_t cur = (int64_t)a->w[i] - (i < b->n ? (int64_t)b->w[i] : 0) - br;
        br = cur < 0;
        if (cur < 0) cur += ((int64_t)1 << 32);
        a->w[i] = (uint32_t)cur;
    }
    g66_trim(a);
}
static void g66_adds(g66_bn *a, uint32_t v) {
    g66_bn t = g66_alloc(1);
    t.w[0] = v;
    g66_trim(&t);
    g66_addto(a, &t);
    g66_free(&t);
}
static void g66_subs(g66_bn *a, uint32_t v) {
    g66_bn t = g66_alloc(1);
    t.w[0] = v;
    g66_trim(&t);
    g66_subfrom(a, &t);
    g66_free(&t);
}
static g66_bn g66_mul(const g66_bn *a, const g66_bn *b) {
    if (!a->n || !b->n) return g66_alloc(0);
    g66_bn r = g66_alloc(a->n + b->n);
    for (size_t i = 0; i < a->n; i++) {
        uint64_t c = 0;
        for (size_t j = 0; j < b->n; j++) {
            uint64_t cur = (uint64_t)a->w[i] * b->w[j] + r.w[i + j] + c;
            r.w[i + j] = (uint32_t)cur;
            c = cur >> 32;
        }
        r.w[i + b->n] = (uint32_t)c;
    }
    g66_trim(&r);
    return r;
}
/* u = q*v + r, v != 0 */
static void g66_divmod(const g66_bn *u, const g66_bn *v, g66_bn *q, g66_bn *r) {
    if (g66_cmp(u, v) < 0) { *q = g66_alloc(0); *r = g66_copy(u); return; }
    if (v->n == 1) {
        *q = g66_copy(u);
        uint32_t rem = g66_divs(q, v->w[0]);
        *r = g66_alloc(1);
        r->w[0] = rem;
        g66_trim(r);
        return;
    }
    size_t n = v->n, m = u->n;
    const uint64_t B = (uint64_t)1 << 32;
    int s = 0;
    uint32_t top = v->w[n - 1];
    while (!(top & 0x80000000u)) { top <<= 1; s++; }
    uint32_t *vn = (uint32_t *)calloc(n, sizeof(uint32_t));
    uint32_t *un = (uint32_t *)calloc(m + 1, sizeof(uint32_t));
    for (size_t i = n - 1; i > 0; i--)
        vn[i] = (v->w[i] << s) | (s ? v->w[i - 1] >> (32 - s) : 0);
    vn[0] = v->w[0] << s;
    un[m] = s ? u->w[m - 1] >> (32 - s) : 0;
    for (size_t i = m - 1; i > 0; i--)
        un[i] = (u->w[i] << s) | (s ? u->w[i - 1] >> (32 - s) : 0);
    un[0] = u->w[0] << s;
    *q = g66_alloc(m - n + 1);
    for (size_t j = m - n + 1; j-- > 0;) {
        uint64_t num = ((uint64_t)un[j + n] << 32) | un[j + n - 1];
        uint64_t qhat = num / vn[n - 1], rhat = num % vn[n - 1];
        while (qhat >= B || qhat * vn[n - 2] > ((rhat << 32) | un[j + n - 2])) {
            qhat--;
            rhat += vn[n - 1];
            if (rhat >= B) break;
        }
        int64_t k = 0, t;
        for (size_t i = 0; i < n; i++) {
            uint64_t p = qhat * vn[i];
            t = (int64_t)un[i + j] - k - (int64_t)(p & 0xFFFFFFFFu);
            un[i + j] = (uint32_t)t;
            k = (int64_t)(p >> 32) - (t >> 32);
        }
        t = (int64_t)un[j + n] - k;
        un[j + n] = (uint32_t)t;
        if (t < 0) {
            qhat--;
            uint64_t c = 0;
            for (size_t i = 0; i < n; i++) {
                uint64_t s2 = (uint64_t)un[i + j] + vn[i] + c;
                un[i + j] = (uint32_t)s2;
                c = s2 >> 32;
            }
            un[j + n] += (uint32_t)c;
        }
        q->w[j] = (uint32_t)qhat;
    }
    *r = g66_alloc(n);
    for (size_t i = 0; i < n; i++)
        r->w[i] = (un[i] >> s) | (s ? (uint32_t)((uint64_t)un[i + 1] << (32 - s)) : 0);
    g66_trim(q);
    g66_trim(r);
    free(vn);
    free(un);
}
/* floor-ish arctan(1/m) * 2^W, error < number of terms */
static g66_bn g66_atan(size_t W, uint32_t m) {
    g66_bn t = g66_pow2(W);
    g66_divs(&t, m);
    g66_bn pos = g66_copy(&t), neg = g66_alloc(0);
    uint32_t m2 = m * m;
    uint32_t k = 1;
    for (;;) {
        g66_divs(&t, m2);
        if (!t.n) break;
        g66_bn tmp = g66_copy(&t);
        g66_divs(&tmp, 2 * k + 1);
        if (k & 1) g66_addto(&neg, &tmp); else g66_addto(&pos, &tmp);
        g66_free(&tmp);
        k++;
    }
    g66_subfrom(&pos, &neg);
    g66_free(&t);
    g66_free(&neg);
    return pos;
}
/* one greedy step on x = p/q; returns 0 if x == 0 */
static int g66_step(g66_bn *p, g66_bn *q, g66_bn *d) {
    if (!p->n) return 0;
    g66_bn fl, r;
    g66_divmod(q, p, &fl, &r);
    *d = fl;
    if (r.n) { g66_adds(d, 1); g66_subfrom(p, &r); }
    else p->n = 0;
    g66_bn nq = g66_mul(q, d);
    g66_free(q);
    *q = nq;
    g66_free(&r);
    return 1;
}

static g66_bn *g66_cache = NULL;
static size_t g66_cnt = 0, g66_cap = 0;
static size_t g66_P = 128;

static void g66_run(size_t P) {
    size_t W = P + 64;
    g66_bn S = g66_atan(W, 5);
    g66_muls(&S, 16);
    g66_bn A = g66_atan(W, 239);
    g66_muls(&A, 4);
    g66_subfrom(&S, &A);
    g66_free(&A);
    g66_bn piP = g66_alloc(S.n > 2 ? S.n - 2 : 0);
    for (size_t i = 2; i < S.n; i++) piP.w[i - 2] = S.w[i];
    g66_trim(&piP);
    g66_free(&S);
    g66_bn three = g66_pow2(P);
    g66_muls(&three, 3);
    g66_bn L = g66_copy(&piP), U = g66_copy(&piP);
    g66_subs(&L, 1);
    g66_subfrom(&L, &three);
    g66_adds(&U, 2);
    g66_subfrom(&U, &three);
    g66_bn qL = g66_pow2(P), qU = g66_pow2(P);
    size_t i = 0;
    for (;;) {
        if (!L.n || !U.n) break;
        g66_bn dL, dU;
        g66_step(&L, &qL, &dL);
        g66_step(&U, &qU, &dU);
        int same = g66_cmp(&dL, &dU) == 0;
        g66_free(&dU);
        if (!same) { g66_free(&dL); break; }
        if (i >= g66_cnt) {
            if (g66_cnt == g66_cap) {
                g66_cap = g66_cap ? 2 * g66_cap : 16;
                g66_cache = (g66_bn *)realloc(g66_cache, g66_cap * sizeof(g66_bn));
            }
            g66_cache[g66_cnt++] = dL;
        } else {
            g66_free(&dL);
        }
        i++;
    }
    g66_free(&piP); g66_free(&three);
    g66_free(&L); g66_free(&U); g66_free(&qL); g66_free(&qU);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    for (;;) {
        if (g66_cnt >= (size_t)n) break;
        g66_run(g66_P);
        if (g66_cnt >= (size_t)n) break;
        g66_P *= 2;
    }
    const g66_bn *v = &g66_cache[n - 1];
    seq_t r = 0;
    for (size_t i = v->n; i-- > 0;) {
        r = r * 65536 + (seq_t)(v->w[i] >> 16);
        r = r * 65536 + (seq_t)(v->w[i] & 0xFFFFu);
    }
    return r;
}
