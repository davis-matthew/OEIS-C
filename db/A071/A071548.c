/*
 * A071548  Determinant of the n X n matrix whose element (i,j) equals
 *          f(|i-j|) where f(n) is 1 if the sum of middle divisors (A071090) >
 *          0, else 0.
 * https://oeis.org/A071548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = det(M_n), M_n(i,j) = f(|i-j|), f(k) = [A071090(k) > 0]
 *            (f(0) = 0); the determinant is computed modulo several ~2^31
 *            primes (Gaussian elimination) and rebuilt exactly with balanced
 *            Garner CRT, using the Hadamard bound n^(n/2)
 * Range:     i8 n <= 19, i16 n <= 25, i32 n <= 37, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-07)
 * Helpers:   bit_length_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/bit_length_u64.h"

/* 1 if n has a divisor d with n/2 <= d^2 < 2n (i.e. A071090(n) > 0), else 0 */
static int A071548_f(uint64_t k)
{
    if (k == 0)
        return 0;
    for (uint64_t i = 1; i * i <= k; ++i)
    {
        if (k % i == 0)
        {
            uint64_t d = i;
            if (2 * d * d >= k && d * d < 2 * k)
                return 1;
            uint64_t j = k / i;
            if (j != i && 2 * j * j >= k && j * j < 2 * k)
                return 1;
        }
    }
    return 0;
}

static uint64_t A071548_powmod(uint64_t b, uint64_t e, uint64_t p)
{
    uint64_t r = 1 % p;
    b %= p;
    while (e)
    {
        if (e & 1)
            r = r * b % p;
        b = b * b % p;
        e >>= 1;
    }
    return r;
}

static uint64_t A071548_detmod(const int *fv, size_t N, uint64_t p, uint64_t *M)
{
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            M[i * N + j] = (uint64_t)fv[i > j ? i - j : j - i];
    uint64_t det = 1;
    for (size_t k = 0; k < N; ++k)
    {
        size_t r = k;
        while (r < N && M[r * N + k] == 0)
            ++r;
        if (r == N)
            return 0;
        if (r != k)
        {
            for (size_t j = 0; j < N; ++j)
            {
                uint64_t t = M[k * N + j];
                M[k * N + j] = M[r * N + j];
                M[r * N + j] = t;
            }
            det = (p - det) % p;
        }
        uint64_t piv = M[k * N + k];
        det = det * piv % p;
        uint64_t inv = A071548_powmod(piv, p - 2, p);
        for (size_t i = k + 1; i < N; ++i)
        {
            uint64_t fct = M[i * N + k] * inv % p;
            if (fct == 0)
                continue;
            for (size_t j = k; j < N; ++j)
                M[i * N + j] = (M[i * N + j] + (p - fct) * M[k * N + j]) % p;
        }
    }
    return det;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    int *fv = (int *)malloc(sizeof(int) * (N + 1));
    for (size_t k = 0; k <= N; ++k)
        fv[k] = A071548_f((uint64_t)k);

    /* Hadamard bound for 0/1 matrix: |det| <= n^(n/2) */
    uint64_t bits = (uint64_t)N * (uint64_t)bit_length_u64((uint64_t)N) / 2 + 2;
    size_t np = (size_t)(bits / 30) + 2;

    uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * np);
    int64_t *c = (int64_t *)malloc(sizeof(int64_t) * np);
    uint64_t *M = (uint64_t *)malloc(sizeof(uint64_t) * (N * N + 1));

    uint64_t cand = 2147483647ULL;
    for (size_t i = 0; i < np; ++i)
    {
        while (!is_prime_u64(cand))
            cand -= 2;
        pr[i] = cand;
        cand -= 2;
    }

    for (size_t i = 0; i < np; ++i)
    {
        uint64_t p = pr[i];
        uint64_t r = A071548_detmod(fv, N, p, M);
        uint64_t acc = 0, mult = 1;
        for (size_t j = 0; j < i; ++j)
        {
            int64_t cj = c[j] % (int64_t)p;
            uint64_t cu = (uint64_t)(cj < 0 ? cj + (int64_t)p : cj);
            acc = (acc + cu * mult) % p;
            mult = mult * (pr[j] % p) % p;
        }
        uint64_t ci = (r + p - acc) % p * A071548_powmod(mult, p - 2, p) % p;
        c[i] = (ci > p / 2) ? (int64_t)ci - (int64_t)p : (int64_t)ci;
    }

    size_t top = np;
    while (top > 0 && c[top - 1] == 0)
        --top;
    seq_t v = 0;
    if (top > 0)
    {
        v = (seq_t)c[top - 1];
        for (size_t i = top - 1; i-- > 0;)
            v = v * (seq_t)(int64_t)pr[i] + (seq_t)c[i];
    }

    free(fv);
    free(pr);
    free(c);
    free(M);
    return v;
}
