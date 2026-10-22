/*
 * A002357  Denominators of convergents to cube root of 5.
 * https://oeis.org/A002357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A(n)*a(n-1) + a(n-2), a(-1) = 0, a(0) = 1, where A(n) is
 *            the n-th partial quotient of the continued fraction of 5^(1/3).
 *            The A(n) are computed exactly by Lagrange's method: start with
 *            P(x) = x^3 - 5 and repeatedly take A = floor(the real root of
 *            P), then replace P(x) with x^3*P(A + 1/x)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CR5_H 8

static uint32_t *cr5_c[4];
static uint32_t *cr5_t1 = NULL, *cr5_t2 = NULL;
static size_t cr5_L = 0;
static uint32_t *cr5_A = NULL;
static size_t cr5_n = 0, cr5_cap = 0;

static void cr5_mul(uint32_t *dst, const uint32_t *src, uint32_t t)
{
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < cr5_L; i++) {
        uint64_t p = (uint64_t)src[i] * t + carry;
        dst[i] = (uint32_t)p;
        carry = p >> 32;
    }
}

static void cr5_add(uint32_t *dst, const uint32_t *src)
{
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < cr5_L; i++) {
        uint64_t s = (uint64_t)dst[i] + src[i] + carry;
        dst[i] = (uint32_t)s;
        carry = s >> 32;
    }
}

static int cr5_neg(const uint32_t *x)
{
    return (int)(x[cr5_L - 1] >> 31);
}

/* sign bit of P(t) */
static int cr5_eval_neg(uint32_t t)
{
    int j;
    memcpy(cr5_t1, cr5_c[3], cr5_L * sizeof(uint32_t));
    for (j = 2; j >= 0; j--) {
        cr5_mul(cr5_t1, cr5_t1, t);
        cr5_add(cr5_t1, cr5_c[j]);
    }
    return cr5_neg(cr5_t1);
}

static void cr5_fit(void)
{
    int ok = 1, j;
    size_t i;
    for (j = 0; j < 4 && ok; j++) {
        uint32_t ext = (cr5_c[j][cr5_L - CR5_H - 1] >> 31) ? 0xFFFFFFFFu : 0u;
        for (i = cr5_L - CR5_H; i < cr5_L; i++)
            if (cr5_c[j][i] != ext) { ok = 0; break; }
    }
    if (ok) return;
    {
        size_t nl = cr5_L * 2;
        for (j = 0; j < 4; j++) {
            uint32_t ext;
            cr5_c[j] = (uint32_t *)realloc(cr5_c[j], nl * sizeof(uint32_t));
            ext = (cr5_c[j][cr5_L - 1] >> 31) ? 0xFFFFFFFFu : 0u;
            for (i = cr5_L; i < nl; i++) cr5_c[j][i] = ext;
        }
        cr5_t1 = (uint32_t *)realloc(cr5_t1, nl * sizeof(uint32_t));
        cr5_t2 = (uint32_t *)realloc(cr5_t2, nl * sizeof(uint32_t));
        cr5_L = nl;
    }
}

static void cr5_init(void)
{
    int j;
    cr5_L = 16;
    for (j = 0; j < 4; j++)
        cr5_c[j] = (uint32_t *)calloc(cr5_L, sizeof(uint32_t));
    cr5_t1 = (uint32_t *)calloc(cr5_L, sizeof(uint32_t));
    cr5_t2 = (uint32_t *)calloc(cr5_L, sizeof(uint32_t));
    cr5_c[3][0] = 1;
    memset(cr5_c[0], 0xFF, cr5_L * sizeof(uint32_t));
    cr5_c[0][0] = 0xFFFFFFFBu; /* -5 */
}

static void cr5_step(void)
{
    int ln, k, j;
    uint64_t lo = 1, hi = 2;
    uint32_t av;
    uint32_t *tp;
    cr5_fit();
    ln = cr5_neg(cr5_c[3]);
    while (hi < 0x80000000ull && cr5_eval_neg((uint32_t)hi) != ln) {
        lo = hi;
        hi *= 2;
    }
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (cr5_eval_neg((uint32_t)mid) != ln) lo = mid; else hi = mid;
    }
    av = (uint32_t)lo;
    /* Taylor shift: P(x) -> P(x + av) */
    for (k = 0; k <= 2; k++)
        for (j = 2; j >= k; j--) {
            cr5_mul(cr5_t2, cr5_c[j + 1], av);
            cr5_add(cr5_c[j], cr5_t2);
        }
    /* reverse: x^3 P(1/x) */
    tp = cr5_c[0]; cr5_c[0] = cr5_c[3]; cr5_c[3] = tp;
    tp = cr5_c[1]; cr5_c[1] = cr5_c[2]; cr5_c[2] = tp;
    if (cr5_n >= cr5_cap) {
        cr5_cap = cr5_cap ? cr5_cap * 2 : 64;
        cr5_A = (uint32_t *)realloc(cr5_A, cr5_cap * sizeof(uint32_t));
    }
    cr5_A[cr5_n++] = av;
}

seq_t a(idx_t n)
{
    seq_t qm = 0, q = 1;
    idx_t k;
    if (cr5_L == 0) cr5_init();
    while ((idx_t)cr5_n <= n) cr5_step();
    for (k = 1; k <= n; k++) {
        seq_t nq = (seq_t)cr5_A[k] * q + qm;
        qm = q;
        q = nq;
    }
    return q;
}
