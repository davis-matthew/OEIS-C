/*
 * A001672  a(n) = floor(Pi^n).
 * https://oeis.org/A001672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(Pi^n); Pi via Machin's formula Pi = 16*arctan(1/5)
 *            - 4*arctan(1/239) in multi-limb fixed point, then raised to the
 *            n-th power by repeated squaring with enough guard bits that the
 *            floor is certain (precision is raised if it is not)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 268 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void pin_add(uint32_t *a, const uint32_t *b, size_t len)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        uint64_t t = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void pin_divsmall(uint32_t *x, size_t len, uint32_t d)
{
    uint64_t r = 0;
    size_t i = len;
    while (i > 0) {
        uint64_t cur;
        i--;
        cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static int pin_iszero(const uint32_t *x, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++)
        if (x[i]) return 0;
    return 1;
}

static void pin_sub(uint32_t *a, const uint32_t *b, size_t len)
{
    uint64_t br = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        uint64_t t = (uint64_t)a[i] - b[i] - br;
        a[i] = (uint32_t)t;
        br = (t >> 63) & 1;
    }
}

/* sum = arctan(1/x) in fixed point, M limbs, top limb is the integer part */
static void pin_atan_inv(uint32_t *sum, size_t M, uint32_t x)
{
    uint32_t *term = (uint32_t *)calloc(M, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(M, sizeof(uint32_t));
    uint64_t k = 1;
    int sign = -1;
    term[M - 1] = 1;
    pin_divsmall(term, M, x);
    memcpy(sum, term, M * sizeof(uint32_t));
    while (1) {
        pin_divsmall(term, M, x);
        pin_divsmall(term, M, x);
        if (pin_iszero(term, M)) break;
        memcpy(t, term, M * sizeof(uint32_t));
        pin_divsmall(t, M, (uint32_t)(2 * k + 1));
        if (sign < 0) pin_sub(sum, t, M);
        else pin_add(sum, t, M);
        sign = -sign;
        k++;
    }
    free(term);
    free(t);
}

/* out = (x*y) >> (32*F), all arrays of L limbs; tmp has 2L limbs */
static void pin_mul(const uint32_t *x, const uint32_t *y, uint32_t *out,
                    uint32_t *tmp, size_t L, size_t F)
{
    size_t lx = L, ly = L, i, j;
    while (lx > 0 && x[lx - 1] == 0) lx--;
    while (ly > 0 && y[ly - 1] == 0) ly--;
    memset(tmp, 0, 2 * L * sizeof(uint32_t));
    for (i = 0; i < lx; i++) {
        uint64_t c = 0;
        uint64_t xi = x[i];
        if (xi == 0) continue;
        for (j = 0; j < ly; j++) {
            uint64_t t = xi * y[j] + tmp[i + j] + c;
            tmp[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        j = i + ly;
        while (c && j < 2 * L) {
            uint64_t t = (uint64_t)tmp[j] + c;
            tmp[j] = (uint32_t)t;
            c = t >> 32;
            j++;
        }
    }
    memcpy(out, tmp + F, L * sizeof(uint32_t));
}

static void pin_mulsmall(uint32_t *x, size_t len, uint32_t m)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

seq_t a(idx_t n)
{
    uint64_t ib, lgn = 0, m;
    size_t I, g;
    if (n == 0) return (seq_t)1;
    ib = (uint64_t)n * 1652 / 1000 + 3;
    I = (size_t)(ib / 32) + 2;
    for (m = 4 * (uint64_t)n + 4; m; m >>= 1) lgn++;
    for (g = 0;; g++) {
        size_t k = g + 1;
        size_t F = (size_t)((ib + lgn + 8 + 31) / 32) + k + 1;
        size_t L = I + F, M = F + 3, i;
        uint32_t *A = (uint32_t *)calloc(M, sizeof(uint32_t));
        uint32_t *B = (uint32_t *)calloc(M, sizeof(uint32_t));
        uint32_t *P = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *R = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *tmp = (uint32_t *)calloc(2 * L, sizeof(uint32_t));
        int top, b, allz = 1, allo = 1;
        seq_t r = 0;

        pin_atan_inv(A, M, 5);
        pin_mulsmall(A, M, 16);
        pin_atan_inv(B, M, 239);
        pin_mulsmall(B, M, 4);
        pin_sub(A, B, M);
        memcpy(P, A + 2, (F + 1) * sizeof(uint32_t));

        memcpy(R, P, L * sizeof(uint32_t));
        top = 63;
        while (!((n >> top) & 1)) top--;
        for (b = top - 1; b >= 0; b--) {
            pin_mul(R, R, T, tmp, L, F);
            memcpy(R, T, L * sizeof(uint32_t));
            if ((n >> b) & 1) {
                pin_mul(R, P, T, tmp, L, F);
                memcpy(R, T, L * sizeof(uint32_t));
            }
        }

        for (i = 0; i < k; i++) {
            uint32_t v = R[F - 1 - i];
            if (v != 0) allz = 0;
            if (v != 0xFFFFFFFFu) allo = 0;
        }
        if (!allz && !allo) {
            size_t li = L;
            while (li > F && R[li - 1] == 0) li--;
            while (li > F) {
                uint32_t v;
                int s;
                li--;
                v = R[li];
                for (s = 24; s >= 0; s -= 8) {
                    r *= 256;
                    r += (unsigned)((v >> s) & 0xFF);
                }
            }
        }
        free(A); free(B); free(P); free(R); free(T); free(tmp);
        if (!allz && !allo) return r;
    }
}
