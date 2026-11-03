/*
 * A001675  a(n) = round(sqrt( 2*Pi )^n).
 * https://oeis.org/A001675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(sqrt(2*Pi)^n), computed with exact multi-precision
 *            fixed-point arithmetic: Pi by Machin's formula, sqrt(2*Pi) by
 *            Newton iteration y <- y*(3 - x*y^2)/2 for 1/sqrt(x), then binary
 *            powering
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 490 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void a1675_add(uint32_t *a, const uint32_t *b, size_t L)
{
    uint64_t c = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t s = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)s;
        c = s >> 32;
    }
}

static void a1675_divu(uint32_t *x, size_t L, uint32_t d)
{
    uint64_t r = 0;
    for (size_t i = L; i-- > 0;) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static int a1675_iszero(const uint32_t *x, size_t L)
{
    for (size_t i = 0; i < L; i++)
        if (x[i]) return 0;
    return 1;
}

static void a1675_sub(uint32_t *a, const uint32_t *b, size_t L)
{
    uint64_t br = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t s = (uint64_t)a[i] - b[i] - br;
        a[i] = (uint32_t)s;
        br = (s >> 32) ? 1 : 0;
    }
}

/* sum += or -= mult * atan(1/x), fixed point with FQ fractional limbs */
static void a1675_atan(uint32_t *sum, uint32_t *t, uint32_t *term, size_t FQ,
                       uint32_t mult, uint32_t x, int neg)
{
    size_t L = FQ + 1;
    uint32_t x2 = x * x;
    memset(t, 0, L * sizeof(uint32_t));
    t[FQ] = mult;
    a1675_divu(t, L, x);
    for (uint32_t k = 0;; k++) {
        if (a1675_iszero(t, L)) break;
        memcpy(term, t, L * sizeof(uint32_t));
        a1675_divu(term, L, 2 * k + 1);
        if ((int)(k & 1) ^ neg) a1675_sub(sum, term, L);
        else a1675_add(sum, term, L);
        a1675_divu(t, L, x2);
    }
}

/* out = (a*b) >> (32*F), arrays of W limbs; out may alias a or b */
static void a1675_mulfx(const uint32_t *a, const uint32_t *b, uint32_t *out,
                        size_t W, size_t F, uint32_t *tmp)
{
    size_t la = W, lb = W;
    while (la > 0 && a[la - 1] == 0) la--;
    while (lb > 0 && b[lb - 1] == 0) lb--;
    memset(tmp, 0, 2 * W * sizeof(uint32_t));
    for (size_t i = 0; i < la; i++) {
        uint64_t c = 0;
        uint64_t ai = a[i];
        if (!ai) continue;
        for (size_t j = 0; j < lb; j++) {
            uint64_t cur = tmp[i + j] + ai * b[j] + c;
            tmp[i + j] = (uint32_t)cur;
            c = cur >> 32;
        }
        tmp[i + lb] = (uint32_t)c;
    }
    for (size_t k = 0; k < W; k++)
        out[k] = (k + F < 2 * W) ? tmp[k + F] : 0;
}

static void a1675_shr1(uint32_t *x, size_t L)
{
    for (size_t i = 0; i < L; i++) {
        uint32_t hi = (i + 1 < L) ? x[i + 1] : 0;
        x[i] = (x[i] >> 1) | (hi << 31);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    int nbits = 0;
    for (idx_t m = n; m; m >>= 1) nbits++;

    uint64_t vbits = (uint64_t)n * 1358 / 1024 + 4;      /* bits of the value */
    size_t F = (size_t)((vbits + 128 + 2 * (uint64_t)nbits) / 32 + 1);
    size_t W = F + (size_t)(vbits / 32) + 3;
    size_t FQ = F + 2;
    size_t LQ = FQ + 1;

    uint32_t *pi = (uint32_t *)calloc(LQ, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(LQ, sizeof(uint32_t));
    uint32_t *term = (uint32_t *)calloc(LQ, sizeof(uint32_t));
    uint32_t *X = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *y = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *u = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *v = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *R = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(2 * W, sizeof(uint32_t));

    /* 2*Pi = 32*atan(1/5) - 8*atan(1/239) */
    a1675_atan(pi, t, term, FQ, 32, 5, 0);
    a1675_atan(pi, t, term, FQ, 8, 239, 1);
    for (size_t i = 0; i <= F; i++) X[i] = pi[i + 2];

    /* Newton for y = 1/sqrt(X), starting at 0.4 */
    y[F] = 2;
    a1675_divu(y, W, 5);
    int iters = 3;
    for (uint64_t prec = 8; prec < 32 * (uint64_t)F + 64; prec *= 2) iters++;
    for (int it = 0; it < iters; it++) {
        a1675_mulfx(y, y, v, W, F, tmp);
        a1675_mulfx(v, X, v, W, F, tmp);
        memset(u, 0, W * sizeof(uint32_t));
        u[F] = 3;
        a1675_sub(u, v, W);
        a1675_mulfx(y, u, y, W, F, tmp);
        a1675_shr1(y, W);
    }
    a1675_mulfx(X, y, S, W, F, tmp);   /* S = sqrt(2*Pi) */

    /* R = S^n */
    memcpy(R, S, W * sizeof(uint32_t));
    for (int i = nbits - 2; i >= 0; i--) {
        a1675_mulfx(R, R, R, W, F, tmp);
        if ((n >> i) & 1) a1675_mulfx(R, S, R, W, F, tmp);
    }

    /* round: add 1/2 */
    {
        uint64_t c = 0x80000000u;
        for (size_t i = F - 1; i < W && c; i++) {
            uint64_t s = (uint64_t)R[i] + c;
            R[i] = (uint32_t)s;
            c = s >> 32;
        }
    }

    size_t top = W;
    while (top > F && R[top - 1] == 0) top--;
    seq_t r = 0;
    for (size_t i = top; i-- > F;) {
        r = r * 65536u + (seq_t)(R[i] >> 16);
        r = r * 65536u + (seq_t)(R[i] & 0xFFFFu);
    }

    free(pi); free(t); free(term); free(X); free(y);
    free(u); free(v); free(S); free(R); free(tmp);
    return r;
}
