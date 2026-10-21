/*
 * A061334  Primes with 22 as smallest positive primitive root.
 * https://oeis.org/A061334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061334 – primes with 22 as the smallest positive primitive root
   a(n) = the nth prime p such that
   ord_p(22) = p-1 and for every integer 2 ≤ k < 22,
   ord_p(k) ≠ p-1.
   The value is returned as type seq_t. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime test – supplied by the OEIS framework */
#include "../utils/is_prime64.h"

/* Prime factorisation of a 64‑bit integer – supplied by the framework */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n,
                                     uint64_t *pr, uint64_t *exp,
                                     size_t *cnt); */

/* Fast modular exponentiation (binary exponentiation) */
static uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1;
    }
    return result;
}

/* Compute multiplicative order of a modulo p.
   The array fac contains the distinct prime factors of p‑1. */
static uint64_t multiplicative_order(uint64_t a, uint64_t p,
                                     const uint64_t *fac, size_t fac_cnt)
{
    uint64_t ord = p - 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t q = fac[i];
        while (ord % q == 0 && modpow(a, ord / q, p) == 1)
            ord /= q;
    }
    return ord;
}

/*-------------  Cache of previously found terms  ----------------------*/
static seq_t *cache = NULL;     /* array of found primes */
static size_t cache_len   = 0;   /* number of stored terms      */
static size_t cache_cap   = 0;   /* allocated capacity         */
static uint64_t last_checked = 1;/* last prime candidate tested */

/* Ensure that cache contains at least n computed terms. */
static void ensure_cache_size(idx_t n)
{
    while (cache_len < (size_t)n) {
        /* Find next prime candidate.  Skip even numbers after 2. */
        uint64_t p = last_checked + 1;
        if (p <= 2)
            p = 2;
        else if ((p & 1ULL) == 0)
            p++;

        /* Find the next prime. */
        while (!is_prime64(p)) {
            p += (p == 2 ? 1ULL : 2ULL);      /* skip even numbers */
        }
        last_checked = p;

        /* 22 must be coprime to p – skip p = 2 or 11. */
        if (22ULL % p == 0ULL)
            continue;

        /* Factor p−1 to obtain its distinct prime factors. */
        uint64_t pr[64], exp[64];
        size_t pcnt = 0;
        factor_u64(p - 1, pr, exp, &pcnt);   /* pr contains distinct primes */

        /* Is 22 a primitive root modulo p? */
        uint64_t order22 = multiplicative_order(22ULL, p, pr, pcnt);
        if (order22 != p - 1)
            continue;   /* 22 is not a primitive root */

        /* Is 22 the smallest primitive root? */
        bool good = true;
        for (uint64_t k = 2; k < 22 && good; ++k) {
            if (multiplicative_order(k, p, pr, pcnt) == p - 1)
                good = false;
        }
        if (!good)
            continue;   /* a smaller integer is a primitive root */

        /* Store the prime in the cache. */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                free(cache);
                exit(1);          /* allocation failure */
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)p;
    }
}

/* Main entry point for the sequence.  a(1) is the first prime listed. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n == 0 */
    ensure_cache_size(n);
    return cache[n - 1];
}
