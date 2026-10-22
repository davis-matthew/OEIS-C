/*
 * A007102  Number of labeled disconnected trivalent (or cubic) graphs with 2n
 *          nodes.
 * https://oeis.org/A007102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002829(n) - A004109(n) = Sum_{i=1..n-1}
 *            binomial(2n-1,2i-1)*A004109(i)*A002829(n-i), a(0) = 1, where
 *            A004109(i) = A002829(i) - a(i) and A002829 is computed by
 *            inclusion-exclusion over loops / double / triple edges in the
 *            configuration model: A002829(n) = (1/6^(2n)) * Sum_{c}
 *            binomial(2n,2...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n; int neg; } bn;

static void bn_trim(bn *a)
{
    while (a->n && a->d[a->n - 1] == 0) a->n--;
    if (!a->n) a->neg = 0;
}

static bn bn_small(uint64_t v)
{
    bn r;
    r.d = (uint32_t *)malloc(2 * sizeof(uint32_t));
    r.n = 0;
    r.neg = 0;
    while (v) { r.d[r.n++] = (uint32_t)v; v >>= 32; }
    return r;
}

static bn bn_copy(const bn *a)
{
    bn r;
    r.d = (uint32_t *)malloc((a->n ? a->n : 1) * sizeof(uint32_t));
    if (a->n) memcpy(r.d, a->d, a->n * sizeof(uint32_t));
    r.n = a->n;
    r.neg = a->neg;
    return r;
}

static void bn_free(bn *a) { free(a->d); a->d = NULL; a->n = 0; a->neg = 0; }

static void bn_negate(bn *a) { if (a->n) a->neg ^= 1; }

static bn bn_mul(const bn *a, const bn *b)
{
    bn r;
    size_t n = a->n + b->n, i, j;
    r.d = (uint32_t *)calloc(n ? n : 1, sizeof(uint32_t));
    r.n = n;
    r.neg = a->neg ^ b->neg;
    for (i = 0; i < a->n; i++) {
        uint64_t carry = 0, ai = a->d[i];
        for (j = 0; j < b->n; j++) {
            uint64_t t = ai * b->d[j] + r.d[i + j] + carry;
            r.d[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        r.d[i + b->n] = (uint32_t)carry;
    }
    bn_trim(&r);
    return r;
}

static void bn_mulsmall(bn *a, uint32_t m)
{
    size_t i;
    uint64_t carry = 0;
    if (!a->n) return;
    a->d = (uint32_t *)realloc(a->d, (a->n + 1) * sizeof(uint32_t));
    for (i = 0; i < a->n; i++) {
        uint64_t t = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) a->d[a->n++] = (uint32_t)carry;
    bn_trim(a);
}

static void bn_divsmall(bn *a, uint32_t m)
{
    uint64_t rem = 0;
    size_t i = a->n;
    while (i-- > 0) {
        uint64_t cur = (rem << 32) | a->d[i];
        a->d[i] = (uint32_t)(cur / m);
        rem = cur % m;
    }
    bn_trim(a);
}

static int bn_magcmp(const bn *a, const bn *b)
{
    size_t i;
    if (a->n != b->n) return a->n > b->n ? 1 : -1;
    i = a->n;
    while (i-- > 0)
        if (a->d[i] != b->d[i]) return a->d[i] > b->d[i] ? 1 : -1;
    return 0;
}

static void bn_addto(bn *a, const bn *x)
{
    size_t i;
    if (!x->n) return;
    if (!a->n) { free(a->d); *a = bn_copy(x); return; }
    if (a->neg == x->neg) {
        size_t n = a->n > x->n ? a->n : x->n;
        uint64_t carry = 0;
        a->d = (uint32_t *)realloc(a->d, (n + 1) * sizeof(uint32_t));
        for (i = a->n; i < n; i++) a->d[i] = 0;
        for (i = 0; i < n; i++) {
            uint64_t s = (uint64_t)a->d[i] + (i < x->n ? x->d[i] : 0) + carry;
            a->d[i] = (uint32_t)s;
            carry = s >> 32;
        }
        a->d[n] = (uint32_t)carry;
        a->n = n + 1;
        bn_trim(a);
    } else {
        int c = bn_magcmp(a, x);
        uint64_t borrow = 0;
        if (c == 0) { a->n = 0; a->neg = 0; return; }
        if (c > 0) {
            for (i = 0; i < a->n; i++) {
                uint64_t t = (uint64_t)a->d[i] - (i < x->n ? x->d[i] : 0) - borrow;
                a->d[i] = (uint32_t)t;
                borrow = (t >> 63) & 1;
            }
            bn_trim(a);
        } else {
            uint32_t *r = (uint32_t *)malloc(x->n * sizeof(uint32_t));
            for (i = 0; i < x->n; i++) {
                uint64_t t = (uint64_t)x->d[i] - (i < a->n ? a->d[i] : 0) - borrow;
                r[i] = (uint32_t)t;
                borrow = (t >> 63) & 1;
            }
            free(a->d);
            a->d = r;
            a->n = x->n;
            a->neg = x->neg;
            bn_trim(a);
        }
    }
}

static bn *Hs = NULL;
static size_t Hcnt = 0;
static bn *Ws = NULL, *Gs = NULL, *Cs = NULL, *Ds = NULL;
static size_t Jcnt = 0;

static void ensure(size_t jmax)
{
    size_t pmax = 2 * jmax, j;
    if (jmax < Jcnt) return;
    if (Hcnt < pmax + 1) {
        Hs = (bn *)realloc(Hs, (pmax + 1) * sizeof(bn));
        while (Hcnt <= pmax) {
            size_t p = Hcnt;
            if (p == 0) {
                Hs[0] = bn_small(1);
            } else if (p == 1) {
                Hs[1] = bn_small(3);
                bn_negate(&Hs[1]);
            } else {
                bn t1 = bn_copy(&Hs[p - 1]);
                bn t2 = bn_copy(&Hs[p - 2]);
                bn_mulsmall(&t1, 3);
                bn_negate(&t1);
                bn_mulsmall(&t2, (uint32_t)(18 * (p - 1)));
                bn_negate(&t2);
                bn_addto(&t1, &t2);
                bn_free(&t2);
                Hs[p] = t1;
            }
            Hcnt++;
        }
    }
    Ws = (bn *)realloc(Ws, (jmax + 1) * sizeof(bn));
    Gs = (bn *)realloc(Gs, (jmax + 1) * sizeof(bn));
    Cs = (bn *)realloc(Cs, (jmax + 1) * sizeof(bn));
    Ds = (bn *)realloc(Ds, (jmax + 1) * sizeof(bn));
    for (j = Jcnt; j <= jmax; j++) {
        size_t k = 2 * j, N = 2 * j, m, c, i, r;
        bn T, W, S, E, D, B;

        /* W(k) */
        T = bn_small(1);
        for (i = 1; i < k; i += 2) bn_mulsmall(&T, (uint32_t)i);
        W = bn_small(0);
        for (m = 0; m <= k; m++) {
            bn term = bn_mul(&T, &Hs[k - m]);
            bn_addto(&W, &term);
            bn_free(&term);
            if (m < k) {
                bn_mulsmall(&T, (uint32_t)(k - m));
                bn_mulsmall(&T, (uint32_t)(2 * m + k + 1));
                bn_divsmall(&T, (uint32_t)(m + 1));
            }
        }
        bn_free(&T);
        Ws[j] = W;

        /* total labeled cubic graphs on N nodes */
        S = bn_small(0);
        E = bn_small(1);
        for (c = 0; c <= j; c++) {
            bn term = bn_mul(&E, &Ws[j - c]);
            bn_addto(&S, &term);
            bn_free(&term);
            if (c < j) {
                bn_mulsmall(&E, (uint32_t)(N - 2 * c));
                bn_mulsmall(&E, (uint32_t)(N - 2 * c - 1));
                bn_mulsmall(&E, 6);
                bn_divsmall(&E, (uint32_t)(c + 1));
            }
        }
        bn_free(&E);
        for (i = 0; i < j; i++) bn_divsmall(&S, 36);
        Gs[j] = S;

        /* disconnected */
        D = bn_small(0);
        B = bn_small(1);
        r = 0;
        for (i = 1; i < j; i++) {
            while (r < 2 * i - 1) {
                bn_mulsmall(&B, (uint32_t)(N - 1 - r));
                bn_divsmall(&B, (uint32_t)(r + 1));
                r++;
            }
            if (Cs[i].n && Gs[j - i].n) {
                bn t1 = bn_mul(&B, &Cs[i]);
                bn t2 = bn_mul(&t1, &Gs[j - i]);
                bn_addto(&D, &t2);
                bn_free(&t1);
                bn_free(&t2);
            }
        }
        bn_free(&B);
        Ds[j] = D;

        /* connected */
        {
            bn negD = bn_copy(&D);
            Cs[j] = bn_copy(&Gs[j]);
            bn_negate(&negD);
            bn_addto(&Cs[j], &negD);
            bn_free(&negD);
        }
    }
    Jcnt = jmax + 1;
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    size_t i;
    if (n == 0) return (seq_t)1;
    ensure((size_t)n);
    i = Ds[n].n;
    while (i-- > 0) {
        res = res * (seq_t)65536;
        res = res * (seq_t)65536;
        res = res + (seq_t)(idx_t)Ds[n].d[i];
    }
    return res;
}
