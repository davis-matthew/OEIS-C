/*
 * A002103  Coefficients of expansion of Jacobi nome q in certain powers of
 *          (1/2)*(1 - sqrt(k')) / (1 + sqrt(k')).
 * https://oeis.org/A002103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/(4n)) * Sum_{j=1..n} c(j)*(4(n-j)+1)*a(n-j), a(0) = 1,
 *            where c(j) = 16*f(j-1) - f(j) and f(k) = A036917(k) = [x^k]
 *            (Sum_k C(2k,k)^2 x^k)^2, with f(0) = 1, f(1) = 8, k^3 f(k) =
 *            8(2k-1)(2k^2-2k+1) f(k-1) - 256 (k-1)^3 f(k-2). (Derived as
 *            follows. Write m = lambda/16 = x^4, so that q^4 = m...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   a001028_grow, a001028_z, a2103_is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a001028_z.h"
#include "../utils/a001028_grow.h"
#include "../utils/a2103_is_fixed.h"

#define A2103_FL 12
#define A2103_FN 64

static void fb_mulsmall(uint32_t *x, uint64_t m)
{
    uint64_t carry = 0;
    int i;
    for (i = 0; i < A2103_FL; i++) {
        uint64_t t = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void fb_divsmall(uint32_t *x, uint64_t d)
{
    uint64_t r = 0;
    int i;
    for (i = A2103_FL - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static void fb_add(uint32_t *x, const uint32_t *y)
{
    uint64_t carry = 0;
    int i;
    for (i = 0; i < A2103_FL; i++) {
        uint64_t t = (uint64_t)x[i] + y[i] + carry;
        x[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void fb_sub(uint32_t *x, const uint32_t *y)
{
    uint64_t borrow = 0;
    int i;
    for (i = 0; i < A2103_FL; i++) {
        uint64_t t = (uint64_t)x[i] - y[i] - borrow;
        x[i] = (uint32_t)t;
        borrow = (t >> 32) ? 1 : 0;
    }
}

static void fb_muladd(uint32_t *acc, const uint32_t *x, const uint32_t *y)
{
    int i, j;
    for (i = 0; i < A2103_FL; i++) {
        uint64_t carry = 0;
        if (x[i] == 0) continue;
        for (j = 0; i + j < A2103_FL; j++) {
            uint64_t t = (uint64_t)acc[i + j] + (uint64_t)x[i] * y[j] + carry;
            acc[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
    }
}

static seq_t fixed_vals[A2103_FN + 1];
static int fixed_done = 0;

static void a2103_fixed_init(void)
{
    static uint32_t f[A2103_FN + 1][A2103_FL];
    static uint32_t c[A2103_FN + 1][A2103_FL];
    static uint32_t av[A2103_FN + 1][A2103_FL];
    static uint32_t bv[A2103_FN + 1][A2103_FL];
    uint32_t t[A2103_FL];
    uint64_t k;
    int n, j, i, by;

    memset(f, 0, sizeof f);
    memset(c, 0, sizeof c);
    memset(av, 0, sizeof av);
    memset(bv, 0, sizeof bv);
    f[0][0] = 1;
    f[1][0] = 8;
    for (k = 2; k <= A2103_FN; k++) {
        memcpy(f[k], f[k - 1], sizeof t);
        fb_mulsmall(f[k], 8 * (2 * k - 1) * (2 * k * k - 2 * k + 1));
        memcpy(t, f[k - 2], sizeof t);
        fb_mulsmall(t, 256 * (k - 1) * (k - 1) * (k - 1));
        fb_sub(f[k], t);
        fb_divsmall(f[k], k * k * k);
    }
    for (k = 1; k <= A2103_FN; k++) {
        memcpy(c[k], f[k - 1], sizeof t);
        fb_mulsmall(c[k], 16);
        fb_sub(c[k], f[k]);
    }
    av[0][0] = 1;
    bv[0][0] = 1;
    for (n = 1; n <= A2103_FN; n++) {
        memset(t, 0, sizeof t);
        for (j = 1; j <= n; j++) fb_muladd(t, c[j], bv[n - j]);
        fb_divsmall(t, (uint64_t)(4 * n));
        memcpy(av[n], t, sizeof t);
        memcpy(bv[n], t, sizeof t);
        fb_mulsmall(bv[n], (uint64_t)(4 * n + 1));
    }
    for (n = 0; n <= A2103_FN; n++) {
        seq_t v = 0;
        for (i = A2103_FL - 1; i >= 0; i--)
            for (by = 3; by >= 0; by--)
                v = (seq_t)(v * 256u + ((av[n][i] >> (8 * by)) & 255u));
        fixed_vals[n] = v;
    }
    fixed_done = 1;
}

static seq_t *bF = 0, *bC = 0, *bA = 0, *bB = 0;
static idx_t bcap = 0, bhave = 0;

static void a2103_big_ensure(idx_t n)
{
    idx_t k, j;
    if (n + 1 > bcap) {
        idx_t nc = bcap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 16) nc = 16;
        if (bcap == 0) {
            bF = a001028_z(nc);
            bC = a001028_z(nc);
            bA = a001028_z(nc);
            bB = a001028_z(nc);
        } else {
            bF = a001028_grow(bF, bcap, nc);
            bC = a001028_grow(bC, bcap, nc);
            bA = a001028_grow(bA, bcap, nc);
            bB = a001028_grow(bB, bcap, nc);
        }
        bcap = nc;
    }
    for (k = bhave; k <= n; k++) {
        if (k == 0) {
            bF[0] = 1;
            bC[0] = 0;
            bA[0] = 1;
            bB[0] = 1;
            continue;
        }
        if (k == 1) {
            bF[1] = 8;
        } else {
            seq_t t1 = bF[k - 1] * (seq_t)(8 * (2 * k - 1) * (2 * k * k - 2 * k + 1));
            seq_t t2 = bF[k - 2] * (seq_t)(256 * (k - 1) * (k - 1) * (k - 1));
            bF[k] = (t1 - t2) / (seq_t)(k * k * k);
        }
        bC[k] = bF[k - 1] * 16 - bF[k];
        {
            seq_t s = 0;
            for (j = 1; j <= k; j++) s += bC[j] * bB[k - j];
            bA[k] = s / (seq_t)(4 * k);
            bB[k] = bA[k] * (seq_t)(4 * k + 1);
        }
    }
    if (n + 1 > bhave) bhave = n + 1;
}

seq_t a(idx_t n)
{
    static int mode = -1;
    if (mode < 0) mode = a2103_is_fixed();
    if (mode) {
        if (!fixed_done) a2103_fixed_init();
        if (n > A2103_FN) return 0; /* far beyond any fixed width */
        return fixed_vals[n];
    }
    if (n >= bhave) a2103_big_ensure(n);
    return bA[n];
}
