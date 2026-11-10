/*
 * A219841  Positions of numbers 2^n + 3^n (n >= 0) in A004050.
 * https://oeis.org/A219841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{distinct 2^j + 3^k <= 2^n + 3^n}. Count the pairs (j,k)
 *            as sum_{k=0..n} bitlength(2^n + 3^n - 3^k) using native big
 *            integers, then subtract the repeated values (all of them are
 *            small, at most 259); a sieve below 2^62 finds them
 * Range:     u8 n <= 12, u16 n <= 202, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 980 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *A219841_dups = NULL;
static size_t A219841_ndups = 0;
static bool A219841_init = false;

static int A219841_cmp(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void A219841_build(void) {
    const uint64_t cap = (uint64_t)1 << 62;
    size_t sz = 0, capn = 4096;
    uint64_t *vals = (uint64_t *)malloc(capn * sizeof(uint64_t));
    for (uint64_t p2 = 1; p2 <= cap; p2 *= 2) {
        for (uint64_t p3 = 1; p3 <= cap - p2; p3 *= 3) {
            vals[sz++] = p2 + p3;
            if (p3 > cap / 3) break;
        }
        if (p2 > cap / 2) break;
    }
    qsort(vals, sz, sizeof(uint64_t), A219841_cmp);
    A219841_dups = (uint64_t *)malloc((sz + 1) * sizeof(uint64_t));
    for (size_t i = 1; i < sz; i++)
        if (vals[i] == vals[i - 1]) A219841_dups[A219841_ndups++] = vals[i];
    free(vals);
    A219841_init = true;
}

static uint64_t A219841_bitlen(const uint32_t *x, size_t L) {
    size_t i = L;
    while (i > 0 && x[i - 1] == 0) i--;
    if (i == 0) return 0;
    uint32_t t = x[i - 1];
    uint64_t b = 0;
    while (t) { t >>= 1; b++; }
    return (uint64_t)(i - 1) * 32 + b;
}

seq_t a(idx_t n) {
    if (!A219841_init) A219841_build();
    size_t L = (size_t)(n / 16) + 4;
    uint32_t *V = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *P = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *R = (uint32_t *)calloc(L, sizeof(uint32_t));

    /* V = 3^n */
    V[0] = 1;
    for (idx_t i = 0; i < n; i++) {
        uint64_t c = 0;
        for (size_t t = 0; t < L; t++) {
            uint64_t s = (uint64_t)V[t] * 3 + c;
            V[t] = (uint32_t)s;
            c = s >> 32;
        }
    }
    /* V += 2^n */
    {
        size_t t = (size_t)(n / 32);
        uint64_t c = (uint64_t)1 << (n % 32);
        while (c && t < L) {
            uint64_t s = (uint64_t)V[t] + c;
            V[t] = (uint32_t)s;
            c = s >> 32;
            t++;
        }
    }

    uint64_t T = (uint64_t)n + 1; /* k = n: V - 3^n = 2^n */
    P[0] = 1;
    for (idx_t k = 0; k < n; k++) {
        int64_t br = 0;
        for (size_t t = 0; t < L; t++) {
            int64_t d = (int64_t)V[t] - (int64_t)P[t] - br;
            if (d < 0) { d += ((int64_t)1 << 32); br = 1; } else br = 0;
            R[t] = (uint32_t)d;
        }
        T += A219841_bitlen(R, L);
        uint64_t c = 0;
        for (size_t t = 0; t < L; t++) {
            uint64_t s = (uint64_t)P[t] * 3 + c;
            P[t] = (uint32_t)s;
            c = s >> 32;
        }
    }

    uint64_t extras = 0;
    if (n >= 40) {
        extras = A219841_ndups;
    } else {
        uint64_t v64 = (uint64_t)V[0] | ((uint64_t)V[1] << 32);
        for (size_t i = 0; i < A219841_ndups; i++)
            if (A219841_dups[i] <= v64) extras++;
    }
    free(V); free(P); free(R);
    return (seq_t)(T - extras);
}
