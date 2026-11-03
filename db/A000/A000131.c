/*
 * A000131  Number of asymmetrical dissections of n-gon.
 * https://oeis.org/A000131
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (C(n-2) - (n/2)*C(n/2-1) - n*C(floor(n/2)-1) -
 *            (n/3)*C(n/3-1) + n*C(n/4-1) + n*C(n/6-1)) / (2*n), where C =
 *            Catalan (A000108) and C(x) = 0 for noninteger x
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 24, u64 n <= 41, u128 n <= 74,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 914 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void bn_add(uint32_t *a, const uint32_t *b, size_t L)
{
    uint64_t carry = 0;
    size_t j;
    for (j = 0; j < L; j++) {
        uint64_t t = (uint64_t)a[j] + b[j] + carry;
        a[j] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void bn_div_small(uint32_t *r, size_t L, uint64_t d)
{
    uint64_t rem = 0;
    size_t j = L;
    while (j > 0) {
        uint64_t t;
        j--;
        t = (rem << 32) | r[j];
        r[j] = (uint32_t)(t / d);
        rem = t % d;
    }
}

static void bn_mul_small(uint32_t *r, size_t L, uint64_t m)
{
    uint64_t carry = 0;
    size_t j;
    for (j = 0; j < L; j++) {
        uint64_t t = (uint64_t)r[j] * m + carry;
        r[j] = (uint32_t)t;
        carry = t >> 32;
    }
}

/* r = Catalan(k), using C(i+1) = C(i) * 2(2i+1) / (i+2) */
static void bn_catalan(uint32_t *r, size_t L, uint64_t k)
{
    uint64_t i;
    memset(r, 0, L * sizeof(uint32_t));
    r[0] = 1;
    for (i = 0; i < k; i++) {
        bn_mul_small(r, L, 2 * (2 * i + 1));
        bn_div_small(r, L, i + 2);
    }
}

/* acc += mult * Catalan(k) */
static void bn_addcat(uint32_t *acc, uint32_t *tmp, size_t L, uint64_t k, uint64_t mult)
{
    bn_catalan(tmp, L, k);
    bn_mul_small(tmp, L, mult);
    bn_add(acc, tmp, L);
}

static void bn_sub(uint32_t *a, const uint32_t *b, size_t L)
{
    int64_t borrow = 0;
    size_t j;
    for (j = 0; j < L; j++) {
        int64_t t = (int64_t)a[j] - (int64_t)b[j] - borrow;
        if (t < 0) {
            t += ((int64_t)1 << 32);
            borrow = 1;
        } else {
            borrow = 0;
        }
        a[j] = (uint32_t)t;
    }
}

seq_t a(idx_t n)
{
    size_t L = (size_t)((2 * n) / 32 + 4);
    uint32_t *P = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *N = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
    seq_t res = 0;
    size_t j;

    /* positive part */
    bn_addcat(P, T, L, n - 2, 1);
    if (n % 4 == 0) bn_addcat(P, T, L, n / 4 - 1, n);
    if (n % 6 == 0) bn_addcat(P, T, L, n / 6 - 1, n);

    /* negative part */
    if (n % 2 == 0) bn_addcat(N, T, L, n / 2 - 1, n / 2);
    bn_addcat(N, T, L, n / 2 - 1, n);
    if (n % 3 == 0) bn_addcat(N, T, L, n / 3 - 1, n / 3);

    bn_sub(P, N, L);
    bn_div_small(P, L, 2 * n);

    j = L;
    while (j > 0 && P[j - 1] == 0) j--;
    while (j > 0) {
        j--;
        res = res * 65536;
        res = res * 65536;
        res += (seq_t)P[j];
    }

    free(P);
    free(N);
    free(T);
    return res;
}
