/*
 * A002680  Denominators of coefficients of polynomials arising from Chebyshev
 *          quadrature.
 * https://oeis.org/A002680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   G_n(x) = x^n*exp(-n*Sum_{k>=1} x^(-2k)/(2k(2k+1))) = Sum_j c_j
 *            x^(n-2j), c_0 = 1, c_j = -(n/(2j)) * Sum_{k=1..j}
 *            c_{j-k}/(2k+1); a(n) = lcm of denominators of
 *            c_0..c_{floor(n/2)} (common denominator of G_n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   c2680_fac
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/c2680_fac.h"

typedef struct { uint32_t *d; size_t len, cap; int neg; } c2680_bn;

static void c2680_res(c2680_bn *x, size_t c)
{
    if (c > x->cap) {
        size_t nc = x->cap ? x->cap : 4;
        while (nc < c) nc *= 2;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void c2680_trim(c2680_bn *x)
{
    while (x->len && x->d[x->len - 1] == 0) x->len--;
    if (!x->len) x->neg = 0;
}

static void c2680_copy(c2680_bn *dst, const c2680_bn *src)
{
    c2680_res(dst, src->len + 1);
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
    dst->neg = src->neg;
}

static void c2680_muls(c2680_bn *x, uint32_t m)
{
    uint64_t c = 0;
    size_t i;
    if (m == 0) { x->len = 0; x->neg = 0; return; }
    for (i = 0; i < x->len; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) {
        c2680_res(x, x->len + 1);
        x->d[x->len++] = (uint32_t)c;
    }
}

static uint32_t c2680_mods(const c2680_bn *x, uint32_t d)
{
    uint64_t r = 0;
    size_t i = x->len;
    while (i--) r = ((r << 32) | x->d[i]) % d;
    return (uint32_t)r;
}

static void c2680_divs(c2680_bn *x, uint32_t d)
{
    uint64_t r = 0;
    size_t i = x->len;
    while (i--) {
        uint64_t cur = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
    c2680_trim(x);
}

static int c2680_cmpmag(const c2680_bn *a, const c2680_bn *b)
{
    size_t i;
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    i = a->len;
    while (i--)
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    return 0;
}

/* acc += t (signed) */
static void c2680_add(c2680_bn *acc, const c2680_bn *t)
{
    size_t i, L;
    int cm;
    if (!t->len) return;
    if (!acc->len) { c2680_copy(acc, t); return; }
    if (acc->neg == t->neg) {
        uint64_t c = 0;
        L = acc->len > t->len ? acc->len : t->len;
        c2680_res(acc, L + 1);
        for (i = acc->len; i < L; i++) acc->d[i] = 0;
        for (i = 0; i < L; i++) {
            uint64_t s = (uint64_t)acc->d[i] + (i < t->len ? t->d[i] : 0) + c;
            acc->d[i] = (uint32_t)s;
            c = s >> 32;
        }
        acc->len = L;
        if (c) acc->d[acc->len++] = (uint32_t)c;
        return;
    }
    cm = c2680_cmpmag(acc, t);
    if (cm == 0) { acc->len = 0; acc->neg = 0; return; }
    if (cm > 0) {
        int64_t borrow = 0;
        for (i = 0; i < acc->len; i++) {
            int64_t s = (int64_t)acc->d[i] - (int64_t)(i < t->len ? t->d[i] : 0) - borrow;
            if (s < 0) { s += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
            acc->d[i] = (uint32_t)s;
        }
        c2680_trim(acc);
    } else {
        int64_t borrow = 0;
        c2680_res(acc, t->len + 1);
        for (i = acc->len; i < t->len; i++) acc->d[i] = 0;
        acc->len = t->len;
        for (i = 0; i < t->len; i++) {
            int64_t s = (int64_t)t->d[i] - (int64_t)acc->d[i] - borrow;
            if (s < 0) { s += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
            acc->d[i] = (uint32_t)s;
        }
        acc->neg = t->neg;
        c2680_trim(acc);
    }
}

seq_t a(idx_t n)
{
    idx_t m = n / 2, P, i, j, k;
    uint32_t *spf, *pr;
    int *pidx, *De, *e, *dif, *mx;
    int np = 0, fi[40], fe[40], nf, f, p, npj;
    c2680_bn *N, tmp = {0, 0, 0, 0};
    seq_t r = 1;

    if (m == 0) return r;
    P = 2 * m + 2;
    spf = (uint32_t *)calloc(P + 1, sizeof(uint32_t));
    pidx = (int *)calloc(P + 1, sizeof(int));
    pr = (uint32_t *)malloc((P + 1) * sizeof(uint32_t));
    for (i = 2; i <= P; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            pidx[i] = np;
            pr[np++] = (uint32_t)i;
            for (j = i * i; j <= P; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    De = (int *)calloc((size_t)(m + 1) * np, sizeof(int));
    e = (int *)malloc(np * sizeof(int));
    dif = (int *)malloc(np * sizeof(int));
    mx = (int *)calloc(np, sizeof(int));
    N = (c2680_bn *)calloc(m + 1, sizeof(c2680_bn));
    c2680_res(&N[0], 1);
    N[0].d[0] = 1; N[0].len = 1; N[0].neg = 0;

    for (j = 1; j <= m; j++) {
        int *rowj = De + (size_t)j * np;
        c2680_bn *S = &N[j];
        /* only primes <= 2j+1 can occur in denominators up to c_j */
        npj = 0;
        while (npj < np && pr[npj] <= 2 * j + 1) npj++;
        for (p = 0; p < npj; p++) e[p] = 0;
        for (k = 1; k <= j; k++) {
            int *row = De + (size_t)(j - k) * np;
            for (p = 0; p < npj; p++) if (row[p] > e[p]) e[p] = row[p];
            nf = c2680_fac(2 * k + 1, spf, pidx, fi, fe);
            for (f = 0; f < nf; f++)
                if (row[fi[f]] + fe[f] > e[fi[f]]) e[fi[f]] = row[fi[f]] + fe[f];
        }
        S->len = 0; S->neg = 0;
        for (k = 1; k <= j; k++) {
            int *row = De + (size_t)(j - k) * np;
            uint64_t acc = 1;
            int c;
            if (!N[j - k].len) continue;
            for (p = 0; p < npj; p++) dif[p] = e[p] - row[p];
            nf = c2680_fac(2 * k + 1, spf, pidx, fi, fe);
            for (f = 0; f < nf; f++) dif[fi[f]] -= fe[f];
            c2680_copy(&tmp, &N[j - k]);
            for (p = 0; p < npj; p++) {
                for (c = 0; c < dif[p]; c++) {
                    if (acc * pr[p] > 0xFFFFFFFFull) { c2680_muls(&tmp, (uint32_t)acc); acc = 1; }
                    acc *= pr[p];
                }
            }
            if (acc > 1) c2680_muls(&tmp, (uint32_t)acc);
            c2680_add(S, &tmp);
        }
        c2680_muls(S, (uint32_t)n);
        if (S->len) S->neg = !S->neg;
        for (p = 0; p < npj; p++) rowj[p] = e[p];
        nf = c2680_fac(2 * j, spf, pidx, fi, fe);
        for (f = 0; f < nf; f++) rowj[fi[f]] += fe[f];
        if (!S->len) {
            for (p = 0; p < np; p++) rowj[p] = 0;
        } else {
            for (p = 0; p < npj; p++)
                while (rowj[p] > 0 && c2680_mods(S, pr[p]) == 0) {
                    c2680_divs(S, pr[p]);
                    rowj[p]--;
                }
        }
        for (p = 0; p < npj; p++) if (rowj[p] > mx[p]) mx[p] = rowj[p];
    }

    {
        int c;
        for (p = 0; p < np; p++)
            for (c = 0; c < mx[p]; c++) r = r * (seq_t)pr[p];
    }

    for (j = 0; j <= m; j++) free(N[j].d);
    free(N);
    free(tmp.d);
    free(mx);
    free(dif);
    free(e);
    free(De);
    free(pr);
    free(pidx);
    free(spf);
    return r;
}
