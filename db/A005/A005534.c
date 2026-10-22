/*
 * A005534  Decimal expansion of fifth root of 5.
 * https://oeis.org/A005534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(5^(1/5) * 10^(n-1)) mod 10. 5^(1/5) = 5*y^4, where
 *            y = 5^(-1/5) comes from the Newton iteration y <- y + y*(1 -
 *            5*y^5)/5, run in multi-limb fixed point
 * Range:     u8 n <= 9998, u16 n <= 9998, u32 n <= 9998, u64 n <= 9998,
 *            u128 n <= 9998, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LB 1000000000u

static uint32_t *root_res = NULL;
static long root_L = 0;

/* All numbers: L+1 limbs base 1e9, little-endian; value = X / B^L */
static void fx_mul(const uint32_t *a, const uint32_t *b, uint32_t *out, uint32_t *tmp, long L)
{
    long i, j;
    memset(tmp, 0, sizeof(uint32_t) * (size_t)(2 * L + 3));
    for (i = 0; i <= L; i++) {
        uint64_t carry = 0, ai = a[i];
        if (ai == 0) continue;
        for (j = 0; j <= L; j++) {
            uint64_t cur = (uint64_t)tmp[i + j] + ai * b[j] + carry;
            tmp[i + j] = (uint32_t)(cur % LB);
            carry = cur / LB;
        }
        j = i + L + 1;
        while (carry) {
            uint64_t cur = (uint64_t)tmp[j] + carry;
            tmp[j] = (uint32_t)(cur % LB);
            carry = cur / LB;
            j++;
        }
    }
    for (i = 0; i <= L; i++) out[i] = tmp[i + L];
}

static void fx_mul_small(const uint32_t *a, uint32_t k, uint32_t *out, long L)
{
    uint64_t carry = 0;
    long i;
    for (i = 0; i <= L; i++) {
        uint64_t cur = (uint64_t)a[i] * k + carry;
        out[i] = (uint32_t)(cur % LB);
        carry = cur / LB;
    }
}

static void fx_div_small(const uint32_t *a, uint32_t k, uint32_t *out, long L)
{
    uint64_t rem = 0;
    long i;
    for (i = L; i >= 0; i--) {
        uint64_t cur = rem * LB + a[i];
        out[i] = (uint32_t)(cur / k);
        rem = cur % k;
    }
}

static int fx_cmp(const uint32_t *a, const uint32_t *b, long L)
{
    long i;
    for (i = L; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void fx_add(const uint32_t *a, const uint32_t *b, uint32_t *out, long L)
{
    uint32_t carry = 0;
    long i;
    for (i = 0; i <= L; i++) {
        uint32_t s = a[i] + b[i] + carry;
        if (s >= LB) { s -= LB; carry = 1; } else carry = 0;
        out[i] = s;
    }
}

/* out = a - b, requires a >= b */
static void fx_sub(const uint32_t *a, const uint32_t *b, uint32_t *out, long L)
{
    int64_t borrow = 0;
    long i;
    for (i = 0; i <= L; i++) {
        int64_t s = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (s < 0) { s += LB; borrow = 1; } else borrow = 0;
        out[i] = (uint32_t)s;
    }
}

static void compute_root(long L)
{
    size_t sz = sizeof(uint32_t) * (size_t)(L + 1);
    uint32_t *y = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint32_t *y2 = (uint32_t *)malloc(sz);
    uint32_t *y4 = (uint32_t *)malloc(sz);
    uint32_t *t = (uint32_t *)malloc(sz);
    uint32_t *e = (uint32_t *)malloc(sz);
    uint32_t *d = (uint32_t *)malloc(sz);
    uint32_t *one = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(2 * L + 3));
    long prec = 9, iters = 0, it;

    one[L] = 1;
    y[L - 1] = 724779663u; /* 5^(-1/5) ~ 0.724779663 */
    while (prec < 9 * (L + 1)) { prec *= 2; iters++; }
    iters += 3;

    for (it = 0; it < iters; it++) {
        fx_mul(y, y, y2, tmp, L);
        fx_mul(y2, y2, y4, tmp, L);
        fx_mul(y4, y, t, tmp, L);
        fx_mul_small(t, 5, t, L);
        if (fx_cmp(t, one, L) <= 0) {
            fx_sub(one, t, e, L);
            fx_mul(y, e, d, tmp, L);
            fx_div_small(d, 5, d, L);
            fx_add(y, d, y, L);
        } else {
            fx_sub(t, one, e, L);
            fx_mul(y, e, d, tmp, L);
            fx_div_small(d, 5, d, L);
            fx_sub(y, d, y, L);
        }
    }
    fx_mul(y, y, y2, tmp, L);
    fx_mul(y2, y2, y4, tmp, L);
    fx_mul_small(y4, 5, y4, L);

    free(root_res);
    root_res = y4;
    root_L = L;
    free(y); free(y2); free(t); free(e); free(d); free(one); free(tmp);
}

seq_t a(idx_t n)
{
    long need = (long)((n + 8) / 9) + 4;
    long k, limb, p, i;
    uint32_t v;

    if (n == 1) return (seq_t)1;
    if (root_res == NULL || root_L < need) {
        long L = need;
        if (L < 2 * root_L) L = 2 * root_L;
        compute_root(L);
    }
    k = (long)(n - 1);              /* k-th digit after the decimal point */
    limb = root_L - 1 - (k - 1) / 9;
    p = (k - 1) % 9;
    v = root_res[limb];
    for (i = 0; i < 8 - p; i++) v /= 10;
    return (seq_t)(v % 10);
}
