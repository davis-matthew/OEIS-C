/*
 * A001896  Numerators of cosecant numbers -2*(2^(2*n - 1) -
 *          1)*Bernoulli(2*n); also of Bernoulli(2*n, 1/2) and Bernoulli(2*n,
 *          1/4).
 * https://oeis.org/A001896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = (-1)^n * (2^(2n-1) - 1) * 2n * T(2n-1) * D(2n) / (4^n *
 *            (4^n - 1)) for n >= 1, a(0) = 1, where T(2n-1) = A000182(n) are
 *            the tangent numbers and D(2n) = Product_{p prime, (p-1) | 2n} p
 *            is the denominator of Bernoulli(2n); i.e. a(n) = -(2^(2n-1) - 1)
 *            * numerator(Bernoulli(2n))
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 17,
 *            big any n
 * Helpers:   c1896_bitlen, c1896_isprime, c1896_mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/c1896_bitlen.h"
#include "../utils/c1896_isprime.h"
#include "../utils/c1896_mul_small.h"

typedef struct {
    uint32_t *d;
    size_t len;
} c1896_num;

static c1896_num *c1896_T = NULL;
static idx_t c1896_N = 0;

/* y = a*x + b*y */
static void c1896_lin(c1896_num *y, const c1896_num *x, uint64_t a, uint64_t b) {
    size_t L = y->len > x->len ? y->len : x->len;
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t xi = i < x->len ? x->d[i] : 0;
        uint64_t yi = i < y->len ? y->d[i] : 0;
        uint64_t t = a * xi + b * yi + carry;
        y->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) y->d[L++] = (uint32_t)carry;
    y->len = L;
}

static void c1896_build(idx_t N) {
    idx_t j, k;
    if (c1896_T) {
        for (j = 1; j <= c1896_N; j++) free(c1896_T[j].d);
        free(c1896_T);
    }
    c1896_T = (c1896_num *)malloc((size_t)(N + 1) * sizeof(c1896_num));
    for (j = 1; j <= N; j++) {
        uint64_t bits = 4 * (uint64_t)j + 2 * (uint64_t)j * c1896_bitlen(2 * (uint64_t)j);
        size_t cap = (size_t)(bits / 32 + 4);
        c1896_T[j].d = (uint32_t *)calloc(cap, sizeof(uint32_t));
        c1896_T[j].len = 0;
    }
    c1896_T[1].d[0] = 1;
    c1896_T[1].len = 1;
    for (k = 2; k <= N; k++) {
        memcpy(c1896_T[k].d, c1896_T[k - 1].d, c1896_T[k - 1].len * sizeof(uint32_t));
        c1896_T[k].len = c1896_T[k - 1].len;
        c1896_mul_small(c1896_T[k].d, &c1896_T[k].len, (uint32_t)(k - 1));
    }
    for (k = 2; k <= N; k++)
        for (j = k; j <= N; j++)
            c1896_lin(&c1896_T[j], &c1896_T[j - 1], (uint64_t)(j - k), (uint64_t)(j - k + 2));
    c1896_N = N;
}

seq_t a(idx_t n) {
    uint64_t m, e;
    size_t np = 0, cap, len, ylen, zlen, dl, qL, i, j, ls;
    unsigned bs;
    uint32_t *x, *y, *z, *dv, *q, inv, d0;
    seq_t r;

    if (n == 0) return (seq_t)1;
    if (n > c1896_N) c1896_build(n + n / 4 + 16);

    m = 2 * (uint64_t)n;
    for (e = 1; e <= m; e++)
        if (m % e == 0 && c1896_isprime(e + 1)) np++;

    cap = c1896_T[n].len + 2 + np + (size_t)(m / 32) + 6;
    x = (uint32_t *)calloc(cap, sizeof(uint32_t));
    memcpy(x, c1896_T[n].d, c1896_T[n].len * sizeof(uint32_t));
    len = c1896_T[n].len;
    c1896_mul_small(x, &len, (uint32_t)m);
    for (e = 1; e <= m; e++)
        if (m % e == 0 && c1896_isprime(e + 1)) c1896_mul_small(x, &len, (uint32_t)(e + 1));

    /* y = x * 2^(m-1) - x */
    y = (uint32_t *)calloc(cap, sizeof(uint32_t));
    ls = (size_t)((m - 1) / 32);
    bs = (unsigned)((m - 1) % 32);
    for (i = 0; i < len; i++) {
        uint64_t v = (uint64_t)x[i] << bs;
        y[i + ls] |= (uint32_t)v;
        y[i + ls + 1] |= (uint32_t)(v >> 32);
    }
    ylen = len + ls + 2;
    {
        uint64_t borrow = 0;
        for (i = 0; i < ylen; i++) {
            uint64_t xi = i < len ? x[i] : 0;
            uint64_t t = (uint64_t)y[i] - xi - borrow;
            y[i] = (uint32_t)t;
            borrow = (t >> 32) ? 1 : 0;
        }
    }
    while (ylen > 1 && y[ylen - 1] == 0) ylen--;

    /* z = y >> m */
    ls = (size_t)(m / 32);
    bs = (unsigned)(m % 32);
    zlen = ylen > ls ? ylen - ls : 1;
    z = (uint32_t *)calloc(zlen + 1, sizeof(uint32_t));
    for (i = 0; i < zlen && i + ls < ylen; i++) {
        uint32_t lo = y[i + ls] >> bs;
        uint32_t hi = 0;
        if (bs && i + ls + 1 < ylen) hi = y[i + ls + 1] << (32 - bs);
        z[i] = lo | hi;
    }
    while (zlen > 1 && z[zlen - 1] == 0) zlen--;

    /* exact division of z by 2^m - 1 (odd), 2-adically */
    dl = (size_t)((m + 31) / 32);
    dv = (uint32_t *)malloc(dl * sizeof(uint32_t));
    for (i = 0; i < dl; i++) dv[i] = 0xFFFFFFFFu;
    if (m % 32) dv[dl - 1] = (uint32_t)((1u << (m % 32)) - 1u);
    d0 = dv[0];
    inv = d0;
    for (i = 0; i < 5; i++) inv *= 2u - d0 * inv;

    qL = zlen >= dl ? zlen - dl + 1 : 1;
    q = (uint32_t *)calloc(qL, sizeof(uint32_t));
    for (i = 0; i < qL; i++) {
        uint32_t qi = z[i] * inv;
        uint64_t carry = 0, borrow = 0;
        q[i] = qi;
        for (j = 0; i + j < zlen; j++) {
            uint64_t prod, t;
            if (j >= dl && carry == 0 && borrow == 0) break;
            prod = (j < dl ? (uint64_t)qi * dv[j] : 0) + carry;
            carry = prod >> 32;
            t = (uint64_t)z[i + j] - (uint32_t)prod - borrow;
            z[i + j] = (uint32_t)t;
            borrow = (t >> 32) ? 1 : 0;
        }
    }
    while (qL > 1 && q[qL - 1] == 0) qL--;

    r = (seq_t)0;
    for (i = qL; i-- > 0;) {
        r = r * 65536;
        r = r * 65536;
        r += (idx_t)q[i];
    }
    if (n & 1) r = -r;

    free(x);
    free(y);
    free(z);
    free(dv);
    free(q);
    return r;
}
