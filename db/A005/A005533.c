/*
 * A005533  Decimal expansion of fifth root of 4.
 * https://oeis.org/A005533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = floor(10^(n-1) * 4^(1/5)) mod 10; 4^(1/5) = 4*y^4 where y
 *            = 4^(-1/5) is computed in fixed-point base 10^9 by the Newton
 *            iteration y <- y + y*(1 - 4*y^5)/5
 * Range:     u8 n <= 9563, u16 n <= 9563, u32 n <= 9563, u64 n <= 9563,
 *            u128 n <= 9563, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define FX_BASE 1000000000ULL

static uint32_t *fx_X = NULL;   /* 4^(1/5): limbs 0..L-1 fractional, limb L integer */
static long long fx_L = 0;

static void fx_mul(const uint32_t *a, const uint32_t *b, uint32_t *r, long long L)
{
    long long n = L + 1, i, j, k;
    uint32_t *t = (uint32_t *)calloc((size_t)(2 * n + 2), sizeof(uint32_t));
    for (i = 0; i < n; i++) {
        uint64_t c = 0;
        if (a[i] == 0) continue;
        for (j = 0; j < n; j++) {
            uint64_t v = (uint64_t)a[i] * b[j] + t[i + j] + c;
            t[i + j] = (uint32_t)(v % FX_BASE);
            c = v / FX_BASE;
        }
        k = i + n;
        while (c) {
            uint64_t v = (uint64_t)t[k] + c;
            t[k] = (uint32_t)(v % FX_BASE);
            c = v / FX_BASE;
            k++;
        }
    }
    for (k = 0; k <= L; k++) r[k] = t[k + L];
    free(t);
}

static void fx_mulsmall(const uint32_t *a, uint32_t m, uint32_t *r, long long L)
{
    uint64_t c = 0;
    long long i;
    for (i = 0; i <= L; i++) {
        uint64_t v = (uint64_t)a[i] * m + c;
        r[i] = (uint32_t)(v % FX_BASE);
        c = v / FX_BASE;
    }
}

static void fx_div5(uint32_t *a, long long L)
{
    uint64_t rem = 0;
    long long i;
    for (i = L; i >= 0; i--) {
        uint64_t v = rem * FX_BASE + a[i];
        a[i] = (uint32_t)(v / 5);
        rem = v % 5;
    }
}

static int fx_cmp(const uint32_t *a, const uint32_t *b, long long L)
{
    long long i;
    for (i = L; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

/* r = a - b, requires a >= b */
static void fx_sub(const uint32_t *a, const uint32_t *b, uint32_t *r, long long L)
{
    int64_t br = 0;
    long long i;
    for (i = 0; i <= L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - br;
        if (v < 0) { v += (int64_t)FX_BASE; br = 1; } else br = 0;
        r[i] = (uint32_t)v;
    }
}

static void fx_add(const uint32_t *a, const uint32_t *b, uint32_t *r, long long L)
{
    uint64_t c = 0;
    long long i;
    for (i = 0; i <= L; i++) {
        uint64_t v = (uint64_t)a[i] + b[i] + c;
        r[i] = (uint32_t)(v % FX_BASE);
        c = v / FX_BASE;
    }
}

static void fx_compute(long long L)
{
    size_t sz = (size_t)(L + 1) * sizeof(uint32_t);
    uint32_t *y = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint32_t *y2 = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint32_t *y4 = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint32_t *z = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint32_t *d = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint32_t *e = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint32_t *one = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    long long k = 0, it, iters;

    one[L] = 1;
    y[L - 1] = 757858283u;              /* y0 = 0.757858283 ~ 4^(-1/5) */
    while ((1LL << k) < 9 * L) k++;
    iters = k + 4;
    for (it = 0; it < iters; it++) {
        fx_mul(y, y, y2, L);
        fx_mul(y2, y2, y4, L);
        fx_mul(y4, y, z, L);
        fx_mulsmall(z, 4, z, L);
        if (fx_cmp(z, one, L) <= 0) {
            fx_sub(one, z, d, L);
            fx_mul(y, d, e, L);
            fx_div5(e, L);
            fx_add(y, e, y, L);
        } else {
            fx_sub(z, one, d, L);
            fx_mul(y, d, e, L);
            fx_div5(e, L);
            fx_sub(y, e, y, L);
        }
    }
    fx_mul(y, y, y2, L);
    fx_mul(y2, y2, y4, L);
    if (fx_X) free(fx_X);
    fx_X = (uint32_t *)malloc(sz);
    fx_mulsmall(y4, 4, fx_X, L);
    fx_L = L;
    free(y); free(y2); free(y4); free(z); free(d); free(e); free(one);
}

seq_t a(idx_t n)
{
    static const uint32_t p10[9] = {1u, 10u, 100u, 1000u, 10000u, 100000u,
                                    1000000u, 10000000u, 100000000u};
    long long p, need, idx, q;
    if (n <= 1) {
        if (fx_X == NULL) fx_compute(8);
        return (seq_t)fx_X[fx_L];
    }
    p = (long long)(n - 1);            /* fractional digit position, 1-based */
    need = (p + 8) / 9 + 4;            /* limbs incl. guard limbs */
    if (fx_X == NULL || need > fx_L) {
        long long L = need;
        if (fx_X != NULL && 2 * fx_L > L) L = 2 * fx_L;
        if (L < 8) L = 8;
        fx_compute(L);
    }
    idx = fx_L - 1 - (p - 1) / 9;
    q = (p - 1) % 9;
    return (seq_t)((fx_X[idx] / p10[8 - q]) % 10u);
}
