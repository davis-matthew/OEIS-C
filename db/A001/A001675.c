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
 * Helpers:   a1675_add, a1675_atan, a1675_divu, a1675_iszero, a1675_mulfx,
 *            a1675_shr1, a1675_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a1675_add.h"
#include "../utils/a1675_atan.h"
#include "../utils/a1675_divu.h"
#include "../utils/a1675_iszero.h"
#include "../utils/a1675_mulfx.h"
#include "../utils/a1675_shr1.h"
#include "../utils/a1675_sub.h"

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
