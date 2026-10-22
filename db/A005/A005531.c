/*
 * A005531  Decimal expansion of fifth root of 2.
 * https://oeis.org/A005531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(2^(1/5) * 10^(n-1)) mod 10, computed in fixed-point
 *            from 2^(1/5) = (23/20) * (1 - y)^(1/5), y = 36343/6436343, (1 -
 *            y)^(1/5) = Sum_{k>=0} binomial(1/5,k) * (-y)^k
 * Range:     u8 n <= 9955, u16 n <= 9955, u32 n <= 9955, u64 n <= 9955,
 *            u128 n <= 9955, big any n
 * Helpers:   a5531_iszero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a5531_iszero.h"

#define A5531_BASE 1000000000ULL

static unsigned char *a5531_dig = NULL;
static idx_t a5531_ndig = 0;

static void a5531_mul(uint32_t *a, long L, uint64_t m) {
    uint64_t carry = 0;
    long i;
    for (i = L - 1; i >= 0; i--) {
        uint64_t cur = (uint64_t)a[i] * m + carry;
        a[i] = (uint32_t)(cur % A5531_BASE);
        carry = cur / A5531_BASE;
    }
}

static void a5531_div(uint32_t *a, long L, uint64_t d) {
    uint64_t rem = 0;
    long i;
    for (i = 0; i < L; i++) {
        uint64_t cur = rem * A5531_BASE + a[i];
        a[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void a5531_compute(idx_t D) {
    const uint64_t p = 36343ULL, q = 6436343ULL;
    long L = (long)(D / 9) + 5;
    uint32_t *T = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint32_t *R = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint64_t k;
    long i;
    idx_t j;

    /* T = |t_1| = y/5 */
    T[0] = (uint32_t)p;
    a5531_div(T, L, q);
    a5531_div(T, L, 5);

    for (k = 1; !a5531_iszero(T, L); k++) {
        uint64_t carry = 0;
        for (i = L - 1; i >= 0; i--) {
            uint64_t cur = (uint64_t)S[i] + T[i] + carry;
            S[i] = (uint32_t)(cur % A5531_BASE);
            carry = cur / A5531_BASE;
        }
        /* |t_{k+1}| = |t_k| * (5k-1) * p / (5(k+1) q) */
        a5531_mul(T, L, (5 * k - 1) * p);
        a5531_div(T, L, q);
        a5531_div(T, L, 5 * (k + 1));
    }

    /* R = 1 - S */
    R[0] = 1;
    {
        int64_t borrow = 0;
        for (i = L - 1; i >= 0; i--) {
            int64_t cur = (int64_t)R[i] - (int64_t)S[i] - borrow;
            if (cur < 0) { cur += (int64_t)A5531_BASE; borrow = 1; }
            else borrow = 0;
            R[i] = (uint32_t)cur;
        }
    }
    a5531_mul(R, L, 23);
    a5531_div(R, L, 20);

    free(a5531_dig);
    a5531_dig = (unsigned char *)malloc((size_t)D + 1);
    a5531_dig[0] = (unsigned char)(R[0] % 10);
    for (j = 1; j <= D; j++) {
        idx_t li = 1 + (j - 1) / 9;
        int pos = (int)((j - 1) % 9);
        uint32_t v = R[li];
        int s;
        for (s = 0; s < 8 - pos; s++) v /= 10;
        a5531_dig[j] = (unsigned char)(v % 10);
    }
    a5531_ndig = D;

    free(T);
    free(S);
    free(R);
}

seq_t a(idx_t n) {
    idx_t j = n - 1;
    if (a5531_dig == NULL || j > a5531_ndig) {
        idx_t D = 2 * n + 64;
        if (D < 256) D = 256;
        a5531_compute(D);
    }
    return (seq_t)a5531_dig[j];
}
