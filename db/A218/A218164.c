/*
 * A218164  a(n) is the smallest positive integer k such that k^64 + 1 == 0
 *          mod p, where p is the n-th prime of the form 1 + 128*b (see
 *          A208177).
 * https://oeis.org/A218164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least positive k such that k^64 ≡ –1 (mod p), *
 *            where p is the nth prime congruent to 1 modulo 128. * Compute
 *            a(n) by: * 1. Get the nth prime p of the form 1+128*b. * 2. Find
 *            a primitive root g modulo p. * 3. Let b = (p−1)/128. For each
 *            odd m in [1,127], * k = g^(b·m) (mod p). The sma...
 * Range:     u8 none, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   factorize, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/factorize.h"
#include "../utils/mod_pow.h"

/* ------------------------------------------------------------ */
/*  Prime cache of the form 1+128*b                              */
/* ------------------------------------------------------------ */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static size_t prime_cache_cap = 0;

static uint64_t *root_cache = NULL;
static size_t root_cache_cap = 0;

/* Ensure that prime_cache holds at least n primes of the form 1+128*b. */
static void ensure_primes(size_t n)
{
    while (prime_cache_cnt < n) {
        uint64_t cand = (prime_cache_cnt == 0) ? 257ULL
                                              : prime_cache[prime_cache_cnt - 1] + 128ULL;
        while (!is_prime(cand))
            cand += 128ULL;

        if (prime_cache_cnt == prime_cache_cap) {
            size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 32;
            uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                                new_cap * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            prime_cache = tmp;
            prime_cache_cap = new_cap;
        }
        prime_cache[prime_cache_cnt++] = cand;
    }

    /* Expand root_cache in sync with prime_cache. */
    if (root_cache_cap < prime_cache_cap) {
        size_t new_cap = prime_cache_cap;
        uint64_t *tmp = (uint64_t *)realloc(root_cache,
                                            new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        root_cache = tmp;
        root_cache_cap = new_cap;

        /* Initialize new entries to 0 (unknown root). */
        for (size_t i = prime_cache_cnt; i < new_cap; ++i)
            root_cache[i] = 0;
    }
}

/* Find a primitive root modulo the prime p (p > 2). */
static uint64_t find_primitive_root(uint64_t p)
{
    uint64_t phi = p - 1;           /* = 128 * b */
    uint64_t b = phi / 128;

    /* Factor b.  The result contains prime factors with multiplicity. */
    uint64_t fac[64];
    int     exp[64];
    size_t faccnt = factorize(b, fac, exp);

    /* Build the list of distinct prime factors of φ(p). */
    uint64_t qlist[65];
    size_t   qcnt = 0;
    qlist[qcnt++] = 2;                         /* factor from 128 = 2^7 */
    for (size_t i = 0; i < faccnt; ++i) {
        uint64_t q = fac[i];
        bool dup = false;
        for (size_t j = 0; j < qcnt; ++j)
            if (qlist[j] == q) { dup = true; break; }
        if (!dup) qlist[qcnt++] = q;
    }

    /* Test candidates for primitive root. */
    for (uint64_t g = 2; g < p; ++g) {
        bool ok = true;
        for (size_t i = 0; i < qcnt && ok; ++i) {
            uint64_t q = qlist[i];
            if (mod_pow((seq_t)g, phi / q, (seq_t)p) == (seq_t)1)
                ok = false;
        }
        if (ok) return g;
    }
    return 0;   /* Should never occur for a prime > 2. */
}

/* ------------------------------------------------------------ */
/*  Sequence function                                           */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined; safe default. */

    size_t ui = (size_t)n;
    ensure_primes(ui);

    uint64_t p = prime_cache[ui - 1];

    /* Compute (or retrieve) a primitive root modulo p. */
    if (root_cache[ui - 1] == 0)
        root_cache[ui - 1] = find_primitive_root(p);
    uint64_t g = root_cache[ui - 1];

    uint64_t b = (p - 1) / 128;          /* integer division */

    seq_t min_k = (seq_t)(p - 1);        /* large initial value */

    for (uint64_t m = 1; m < 128; m += 2) {
        seq_t k = mod_pow((seq_t)g, b * m, (seq_t)p);
        if (k < min_k)
            min_k = k;
    }
    return min_k;
}
