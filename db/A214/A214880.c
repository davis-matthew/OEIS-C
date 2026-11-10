/*
 * A214880  5-adic valuation of A007185.
 * https://oeis.org/A214880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + v5(k), where k = 5^(-n) mod 2^n (0<k<2^n); this is
 *            the 5-adic valuation of A007185(n) = 5^n*k. Computed with
 *            multi-limb arithmetic mod 2^n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9546 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void a214880_mul(const uint32_t *a, const uint32_t *b, uint32_t *r, size_t L)
{
    size_t i, j;
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    for (i = 0; i < L; i++) {
        uint64_t carry = 0;
        for (j = 0; j + i < L; j++) {
            uint64_t cur = (uint64_t)a[i] * b[j] + t[i + j] + carry;
            t[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
    }
    memcpy(r, t, L * sizeof(uint32_t));
    free(t);
}

seq_t a(idx_t n)
{
    size_t L = (size_t)((n + 31) / 32);
    size_t i;
    int it;
    uint32_t *x = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *f = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *res = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *base = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc(L, sizeof(uint32_t));
    idx_t e;
    unsigned v = 0;

    /* inverse of 5 modulo 2^(32L) via Newton */
    x[0] = 5;
    for (it = 0; it < 14; it++) {
        uint64_t carry = 0;
        int64_t borrow;
        for (i = 0; i < L; i++) {
            uint64_t cur = (uint64_t)x[i] * 5 + carry;
            t[i] = (uint32_t)cur;
            carry = cur >> 32;
        }
        /* f = 2 - t */
        borrow = 0;
        for (i = 0; i < L; i++) {
            int64_t d = (int64_t)(i == 0 ? 2 : 0) - (int64_t)t[i] - borrow;
            if (d < 0) { d += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
            f[i] = (uint32_t)d;
        }
        a214880_mul(x, f, x, L);
    }

    /* k = x^n */
    res[0] = 1;
    memcpy(base, x, L * sizeof(uint32_t));
    e = n;
    while (e > 0) {
        if (e & 1) a214880_mul(res, base, res, L);
        e >>= 1;
        if (e > 0) a214880_mul(base, base, base, L);
    }

    /* reduce mod 2^n */
    {
        size_t full = (size_t)(n / 32);
        unsigned rem = (unsigned)(n % 32);
        if (rem) {
            res[full] &= (((uint32_t)1 << rem) - 1);
        }
    }

    /* 5-adic valuation of k */
    for (;;) {
        uint64_t rem = 0;
        for (i = L; i-- > 0;) {
            uint64_t cur = (rem << 32) | res[i];
            q[i] = (uint32_t)(cur / 5);
            rem = cur % 5;
        }
        if (rem != 0) break;
        memcpy(res, q, L * sizeof(uint32_t));
        v++;
    }

    free(x); free(t); free(f); free(res); free(base); free(q);
    return (seq_t)n + (seq_t)v;
}
