/*
 * A003881  Decimal expansion of Pi/4.
 * https://oeis.org/A003881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy, changed
 * Formula:   Pi/4 = 4*arctan(1/5) - arctan(1/239) (Machin), arctan(1/x) =
 *            Sum_{k>=0} (-1)^k/((2k+1)*x^(2k+1)); a(n) = n-th decimal digit
 *            of Pi/4, computed with base-10^9 fixed-point arithmetic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pd_iszero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pd_iszero.h"

#define BASE 1000000000ULL

static unsigned char *pd_digits = NULL;
static long long pd_cap = 0;

static void pd_div(uint32_t *x, long long len, uint32_t d)
{
    uint64_t r = 0;
    long long i;
    for (i = 0; i < len; i++) {
        uint64_t cur = r * BASE + x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static void pd_mul(uint32_t *x, long long len, uint32_t m)
{
    uint64_t c = 0;
    long long i;
    for (i = len - 1; i >= 0; i--) {
        uint64_t cur = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)(cur % BASE);
        c = cur / BASE;
    }
}

static void pd_add(uint32_t *a, const uint32_t *b, long long len)
{
    uint64_t c = 0;
    long long i;
    for (i = len - 1; i >= 0; i--) {
        uint64_t cur = (uint64_t)a[i] + b[i] + c;
        if (cur >= BASE) { a[i] = (uint32_t)(cur - BASE); c = 1; }
        else { a[i] = (uint32_t)cur; c = 0; }
    }
}

static void pd_sub(uint32_t *a, const uint32_t *b, long long len)
{
    int64_t br = 0;
    long long i;
    for (i = len - 1; i >= 0; i--) {
        int64_t cur = (int64_t)a[i] - (int64_t)b[i] - br;
        if (cur < 0) { cur += (int64_t)BASE; br = 1; } else br = 0;
        a[i] = (uint32_t)cur;
    }
}

static void pd_atan(uint32_t *out, long long len, uint32_t x)
{
    uint32_t *term = (uint32_t *)calloc((size_t)len, sizeof(uint32_t));
    uint32_t *t2 = (uint32_t *)calloc((size_t)len, sizeof(uint32_t));
    uint32_t x2 = x * x;
    uint32_t k = 1;
    term[0] = 1;
    pd_div(term, len, x);
    memcpy(out, term, (size_t)len * sizeof(uint32_t));
    for (;;) {
        pd_div(term, len, x2);
        if (pd_iszero(term, len)) break;
        memcpy(t2, term, (size_t)len * sizeof(uint32_t));
        pd_div(t2, len, 2 * k + 1);
        if (k & 1) pd_sub(out, t2, len);
        else pd_add(out, t2, len);
        k++;
    }
    free(term);
    free(t2);
}

static void pd_compute(long long ndig)
{
    long long limbs = 1 + (ndig + 8) / 9 + 4;
    uint32_t *a5 = (uint32_t *)calloc((size_t)limbs, sizeof(uint32_t));
    uint32_t *a239 = (uint32_t *)calloc((size_t)limbs, sizeof(uint32_t));
    long long i;
    pd_atan(a5, limbs, 5);
    pd_atan(a239, limbs, 239);
    pd_mul(a5, limbs, 4);
    pd_sub(a5, a239, limbs);
    free(pd_digits);
    pd_digits = (unsigned char *)malloc((size_t)ndig);
    for (i = 0; i < ndig; i++) {
        uint32_t limb = a5[1 + i / 9];
        int pos = (int)(i % 9);
        int j;
        for (j = 0; j < 8 - pos; j++) limb /= 10;
        pd_digits[i] = (unsigned char)(limb % 10);
    }
    pd_cap = ndig;
    free(a5);
    free(a239);
}

seq_t a(idx_t n)
{
    seq_t r;
    if ((long long)n >= pd_cap) {
        long long want = 2 * (long long)n + 64;
        if (want < 1024) want = 1024;
        pd_compute(want);
    }
    r = (seq_t)pd_digits[n];
    return r;
}
