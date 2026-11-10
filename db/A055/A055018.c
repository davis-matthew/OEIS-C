/*
 * A055018  Numbers k such that k and k+1 are modest (cf. A054986).
 * https://oeis.org/A055018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   search. A modest number has the form x = A*10^k + B with 0 < B <
 *            10^k, A having D-k digits, A < B and B | A*(10^k - 1). The code
 *            enumerates every such x one digit length D at a time. For each D
 *            and k it loops over the divisors g of 10^k - 1, sets B = g*s
 *            with gcd(s, (10^k-1)/g) = 1, and takes A a...
 * Range:     u8 none, u16 n <= 21, u32 n <= 1325, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 50 terms through n = 1917 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool A055018_is_modest(uint64_t x)
{
    if (x < 10u) return false;
    uint64_t p = 10u;
    for (;;) {
        uint64_t prefix = x / p;
        if (prefix == 0u) break;
        uint64_t suffix = x % p;
        if (suffix != 0u && suffix > prefix && x % suffix == prefix) return true;
        if (p > UINT64_MAX / 10u) break;
        p *= 10u;
    }
    return false;
}

static uint64_t A055018_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static int A055018_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static uint64_t *A055018_tw = NULL;
static size_t A055018_len = 0, A055018_cap = 0;
static uint64_t *A055018_tmp = NULL;
static size_t A055018_tlen = 0, A055018_tcap = 0;
static int A055018_doneD = 1;

static void A055018_push(uint64_t **arr, size_t *len, size_t *cap, uint64_t v)
{
    if (*len >= *cap) {
        size_t nc = *cap ? *cap * 2 : 256;
        *arr = (uint64_t *)realloc(*arr, nc * sizeof(uint64_t));
        *cap = nc;
    }
    (*arr)[(*len)++] = v;
}

/* divisors of n (unsorted); returns count, *out allocated */
static size_t A055018_divisors(uint64_t n, uint64_t **out)
{
    uint64_t pr[64]; int ex[64]; int c = 0;
    uint64_t m = n;
    for (uint64_t d = 2; d <= m / d; d += (d == 2 ? 1 : 2)) {
        if (m % d == 0) {
            pr[c] = d; ex[c] = 0;
            while (m % d == 0) { m /= d; ex[c]++; }
            c++;
        }
    }
    if (m > 1) { pr[c] = m; ex[c] = 1; c++; }
    size_t total = 1;
    for (int i = 0; i < c; i++) total *= (size_t)(ex[i] + 1);
    uint64_t *dv = (uint64_t *)malloc(total * sizeof(uint64_t));
    size_t cnt = 1; dv[0] = 1;
    for (int i = 0; i < c; i++) {
        size_t base = cnt;
        uint64_t pp = 1;
        for (int e = 1; e <= ex[i]; e++) {
            pp *= pr[i];
            for (size_t j = 0; j < base; j++) dv[cnt++] = dv[j] * pp;
        }
    }
    *out = dv;
    return cnt;
}

static void A055018_gen(int D)
{
    uint64_t pw[20];
    pw[0] = 1;
    for (int i = 1; i < 20; i++) pw[i] = pw[i - 1] * 10u;
    A055018_tlen = 0;
    for (int k = (D + 1) / 2; k <= D - 1; k++) {
        uint64_t N = pw[k] - 1u;
        uint64_t P = pw[D - k];
        uint64_t Alo = pw[D - k - 1];
        uint64_t *dv = NULL;
        size_t nd = A055018_divisors(N, &dv);
        for (size_t i = 0; i < nd; i++) {
            uint64_t g = dv[i];
            uint64_t M = N / g;
            for (uint64_t s = 1; s < P && s <= M; s++) {
                if (A055018_gcd(s, M) != 1u) continue;
                uint64_t B = g * s;
                uint64_t Amax = B - 1u;
                if (Amax > P - 1u) Amax = P - 1u;
                if (Amax < Alo) continue;
                uint64_t A = ((Alo + s - 1u) / s) * s;
                for (; A <= Amax; A += s) {
                    uint64_t x = A * pw[k] + B;
                    if (A055018_is_modest(x + 1u))
                        A055018_push(&A055018_tmp, &A055018_tlen, &A055018_tcap, x);
                }
            }
        }
        free(dv);
    }
    if (A055018_tlen > 0) {
        qsort(A055018_tmp, A055018_tlen, sizeof(uint64_t), A055018_cmp);
        for (size_t i = 0; i < A055018_tlen; i++) {
            if (i > 0 && A055018_tmp[i] == A055018_tmp[i - 1]) continue;
            A055018_push(&A055018_tw, &A055018_len, &A055018_cap, A055018_tmp[i]);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A055018_len < (size_t)n && A055018_doneD < 18) {
        A055018_doneD++;
        A055018_gen(A055018_doneD);
    }
    if (A055018_len < (size_t)n) return (seq_t)0;
    return (seq_t)A055018_tw[(size_t)n - 1];
}
