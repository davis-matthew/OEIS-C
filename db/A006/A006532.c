/*
 * A006532  Numbers whose sum of divisors is a square.
 * https://oeis.org/A006532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   segmented sieve of sigma(k); a(n) = n-th k with sigma(k) a
 *            perfect square
 * Range:     u8 n <= 13, u16 n <= 1022, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static uint64_t *terms = NULL;
static size_t nterms = 0, tcap = 0;
static uint64_t next_lo = 1;

static uint32_t *primes = NULL;
static size_t nprimes = 0;
static uint64_t prime_lim = 0;

#define BLOCK 262144

static void ensure_primes(uint64_t lim) {
    if (lim <= prime_lim) return;
    uint64_t nl = prime_lim * 2;
    if (nl < lim) nl = lim;
    if (nl < 1024) nl = 1024;
    char *comp = (char *)calloc((size_t)nl + 1, 1);
    free(primes);
    primes = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)nl / 2 + 16));
    nprimes = 0;
    for (uint64_t i = 2; i <= nl; i++) {
        if (!comp[i]) {
            primes[nprimes++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= nl; j += i) comp[j] = 1;
        }
    }
    free(comp);
    prime_lim = nl;
}

static uint64_t isqrt_u64(uint64_t x) {
    uint64_t r = (uint64_t)sqrt((double)x);
    while (r > 0 && r * r > x) r--;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

static void push_term(uint64_t k) {
    if (nterms == tcap) {
        tcap = tcap ? tcap * 2 : 1024;
        terms = (uint64_t *)realloc(terms, sizeof(uint64_t) * tcap);
    }
    terms[nterms++] = k;
}

static void sieve_block(void) {
    static uint64_t rem[BLOCK], sig[BLOCK];
    uint64_t lo = next_lo, hi = lo + BLOCK;
    uint64_t top = hi - 1;
    uint64_t sq = isqrt_u64(top);
    ensure_primes(sq + 1);
    for (size_t i = 0; i < BLOCK; i++) {
        rem[i] = lo + i;
        sig[i] = 1;
    }
    for (size_t pi = 0; pi < nprimes; pi++) {
        uint64_t p = primes[pi];
        if (p > sq) break;
        uint64_t start = ((lo + p - 1) / p) * p;
        for (uint64_t m = start; m < hi; m += p) {
            size_t idx = (size_t)(m - lo);
            uint64_t r = rem[idx], pe = 1, s = 1;
            while (r % p == 0) {
                r /= p;
                pe *= p;
                s += pe;
            }
            rem[idx] = r;
            sig[idx] *= s;
        }
    }
    for (size_t i = 0; i < BLOCK; i++) {
        uint64_t s = sig[i];
        if (rem[i] > 1) s *= rem[i] + 1;
        uint64_t r = isqrt_u64(s);
        if (r * r == s) push_term(lo + i);
    }
    next_lo = hi;
}

seq_t a(idx_t n) {
    while (nterms < (size_t)n) sieve_block();
    return (seq_t)terms[n - 1];
}
