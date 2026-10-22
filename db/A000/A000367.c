/*
 * A000367  Numerators of Bernoulli numbers B_2n.
 * https://oeis.org/A000367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, nice
 * Formula:   a(n) = numerator(B_{2n}) = (-1)^(n-1) * 2n * T_n * D_n /
 *            (2^(2n)*(2^(2n)-1)), a(0) = 1, where T_n are the tangent numbers
 *            (tan(x) = Sum T_n x^(2n-1)/(2n-1)!), computed by the
 *            Brent-Harvey recurrence T_j <- (j-k)*T_{j-1} + (j-k+2)*T_j, and
 *            D_n = A002445(n) is the product of the primes p with (p-1)...
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 17, i128 n <= 29,
 *            big any n
 * Helpers:   b367_bitlen, b367_isprime, b367_lin, b367_mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/b367_bitlen.h"
#include "../utils/b367_isprime.h"
#include "../utils/b367_lin.h"
#include "../utils/b367_mul_small.h"

static uint32_t **b367_T = NULL;
static size_t *b367_len = NULL;
static size_t b367_N = 0;

static void b367_build(size_t N) {
    size_t j, k;
    uint64_t bits = 0;
    if (b367_T) {
        for (j = 0; j <= b367_N; j++) free(b367_T[j]);
        free(b367_T);
        free(b367_len);
    }
    b367_T = (uint32_t **)calloc(N + 1, sizeof(uint32_t *));
    b367_len = (size_t *)calloc(N + 1, sizeof(size_t));
    for (j = 1; j <= N; j++) {
        size_t cap;
        bits += (uint64_t)b367_bitlen(2 * j - 1) + (uint64_t)b367_bitlen(2 * j);
        cap = (size_t)((bits + 4 * (uint64_t)j + 2) / 32 + 2);
        b367_T[j] = (uint32_t *)calloc(cap, sizeof(uint32_t));
    }
    b367_T[1][0] = 1;
    b367_len[1] = 1;
    for (k = 2; k <= N; k++)
        b367_lin(b367_T[k], &b367_len[k], b367_T[k - 1], b367_len[k - 1], k - 1, 0);
    for (k = 2; k <= N; k++)
        for (j = k; j <= N; j++)
            b367_lin(b367_T[j], &b367_len[j], b367_T[j - 1], b367_len[j - 1],
                     j - k, j - k + 2);
    b367_N = N;
}

seq_t a(idx_t n) {
    size_t nn, lx, cap, i, ld, qlen, sh, ls, bs;
    uint64_t two_n, d, extra_bits;
    uint32_t *X, *D, *Q, d0, inv;
    seq_t r;
    if (n == 0) return (seq_t)1;
    nn = (size_t)n;
    if (nn > b367_N) {
        size_t newN = b367_N + b367_N / 2;
        if (newN < nn) newN = nn;
        b367_build(newN);
    }
    two_n = 2 * (uint64_t)n;

    extra_bits = 64 + (uint64_t)b367_bitlen(n);
    for (d = 1; d <= two_n; d++)
        if (two_n % d == 0) extra_bits += (uint64_t)b367_bitlen(d + 1) + 1;
    cap = b367_len[nn] + (size_t)(extra_bits / 32) + 4;
    X = (uint32_t *)calloc(cap, sizeof(uint32_t));
    memcpy(X, b367_T[nn], b367_len[nn] * sizeof(uint32_t));
    lx = b367_len[nn];

    /* X = T_n * n * D_n */
    b367_mul_small(X, &lx, (uint64_t)n);
    for (d = 1; d <= two_n; d++)
        if (two_n % d == 0 && b367_isprime(d + 1))
            b367_mul_small(X, &lx, d + 1);

    /* X >>= 2n-1 */
    sh = (size_t)(two_n - 1);
    ls = sh / 32;
    bs = sh % 32;
    for (i = 0; i + ls < lx; i++) {
        uint64_t lo = X[i + ls];
        uint64_t hi = (i + ls + 1 < lx) ? X[i + ls + 1] : 0;
        X[i] = (uint32_t)(((hi << 32) | lo) >> bs);
    }
    lx = lx > ls ? lx - ls : 0;
    for (i = lx; i < cap; i++) X[i] = 0;
    while (lx > 0 && X[lx - 1] == 0) lx--;

    /* exact division by 2^(2n) - 1 (odd) */
    ld = (size_t)((two_n + 31) / 32);
    D = (uint32_t *)calloc(ld, sizeof(uint32_t));
    for (i = 0; i < ld; i++) D[i] = 0xFFFFFFFFu;
    if (two_n % 32) D[ld - 1] = (uint32_t)((1ULL << (two_n % 32)) - 1);
    d0 = D[0];
    inv = d0;
    for (i = 0; i < 5; i++) inv *= 2u - d0 * inv;
    qlen = lx >= ld ? lx - ld + 1 : 0;
    Q = (uint32_t *)calloc(qlen + 1, sizeof(uint32_t));
    for (i = 0; i < qlen; i++) {
        uint32_t q = X[i] * inv;
        uint64_t carry = 0, borrow = 0;
        size_t t;
        Q[i] = q;
        for (t = 0; t < ld; t++) {
            uint64_t p = (uint64_t)q * D[t] + carry;
            uint64_t s;
            carry = p >> 32;
            s = (uint64_t)X[i + t] - (uint32_t)p - borrow;
            X[i + t] = (uint32_t)s;
            borrow = (s >> 63) ? 1 : 0;
        }
        for (t = i + ld; t < lx && (carry || borrow); t++) {
            uint64_t s = (uint64_t)X[t] - carry - borrow;
            X[t] = (uint32_t)s;
            carry = 0;
            borrow = (s >> 63) ? 1 : 0;
        }
    }
    while (qlen > 0 && Q[qlen - 1] == 0) qlen--;

    r = 0;
    for (i = qlen; i-- > 0;) {
        r = r * 65536 + (seq_t)(int)(Q[i] >> 16);
        r = r * 65536 + (seq_t)(int)(Q[i] & 0xFFFFu);
    }
    free(X);
    free(D);
    free(Q);
    if (n % 2 == 0) r = -r;
    return r;
}
