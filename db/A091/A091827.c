/*
 * A091827  Least k such that k^n divides A000254(k).
 * https://oeis.org/A091827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 2 such that k^n divides A000254(k), where
 *            A000254(k) = k*A000254(k-1) + (k-1)!, A000254(0) = 0; found by
 *            search using exact bignum arithmetic and p-adic valuations of
 *            A000254(k) for each prime power p^e dividing k
 * Range:     u8 n <= 55, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void a091827_mul_small(uint32_t *x, size_t *len, uint32_t m)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) x[(*len)++] = (uint32_t)c;
}

/* x = x * m + y ; y has length ly; x has capacity for result */
static void a091827_mul_add(uint32_t *x, size_t *len, uint32_t m, const uint32_t *y, size_t ly)
{
    uint64_t c = 0;
    size_t i, n = (*len > ly ? *len : ly);
    for (i = 0; i < n; i++) {
        uint64_t xi = (i < *len) ? x[i] : 0;
        uint64_t yi = (i < ly) ? y[i] : 0;
        uint64_t t = xi * m + yi + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
    while (c) {
        x[i++] = (uint32_t)c;
        c >>= 32;
    }
    *len = i;
}

static uint32_t a091827_rem(const uint32_t *x, size_t len, uint32_t p)
{
    uint64_t r = 0;
    size_t i = len;
    while (i > 0) {
        i--;
        r = ((r << 32) | x[i]) % p;
    }
    return (uint32_t)r;
}

static void a091827_div(uint32_t *x, size_t *len, uint32_t p)
{
    uint64_t r = 0;
    size_t i = *len;
    while (i > 0) {
        i--;
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / p);
        r = cur % p;
    }
    while (*len > 0 && x[*len - 1] == 0) (*len)--;
}

seq_t a(idx_t n)
{
    size_t cap = 64;
    uint32_t *A = (uint32_t *)malloc(cap * sizeof(uint32_t));
    uint32_t *F = (uint32_t *)malloc(cap * sizeof(uint32_t));
    uint32_t *T = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t la, lf;
    uint32_t k;
    uint32_t result = 0;

    /* state after k = 1: A = 1, F = 1! = 1 */
    A[0] = 1; la = 1;
    F[0] = 1; lf = 1;

    for (k = 2;; k++) {
        uint32_t m, p;
        int ok = 1;
        if (la + 4 > cap || lf + 4 > cap) {
            cap *= 2;
            A = (uint32_t *)realloc(A, cap * sizeof(uint32_t));
            F = (uint32_t *)realloc(F, cap * sizeof(uint32_t));
            T = (uint32_t *)realloc(T, cap * sizeof(uint32_t));
        }
        /* A(k) = k*A(k-1) + (k-1)! ; F = (k-1)! now */
        a091827_mul_add(A, &la, k, F, lf);
        a091827_mul_small(F, &lf, k);

        m = k;
        for (p = 2; ok && (uint64_t)p * p <= m; p++) {
            if (m % p == 0) {
                uint64_t e = 0, need, cnt = 0;
                size_t lt;
                while (m % p == 0) { m /= p; e++; }
                need = e * (uint64_t)n;
                memcpy(T, A, la * sizeof(uint32_t));
                lt = la;
                while (cnt < need) {
                    if (a091827_rem(T, lt, p) != 0) break;
                    a091827_div(T, &lt, p);
                    cnt++;
                }
                if (cnt < need) ok = 0;
            }
        }
        if (ok && m > 1) {
            uint64_t need = (uint64_t)n, cnt = 0;
            size_t lt;
            memcpy(T, A, la * sizeof(uint32_t));
            lt = la;
            while (cnt < need) {
                if (a091827_rem(T, lt, m) != 0) break;
                a091827_div(T, &lt, m);
                cnt++;
            }
            if (cnt < need) ok = 0;
        }
        if (ok) { result = k; break; }
    }
    free(A); free(F); free(T);
    return (seq_t)result;
}
