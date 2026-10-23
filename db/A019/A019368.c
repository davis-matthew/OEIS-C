/*
 * A019368  Primes with primitive root 43.
 * https://oeis.org/A019368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A019368: primes p such that 43 is a primitive root modulo p.
    The routine searches successive primes, testing the primitive‑root property
    and memoises the results.  It never hard‑codes terms; in particular it
    filters out p=43 where 43 mod p ≡ 0 and cannot be a unit.  The same
    algorithm works for arbitrary‑precision seq_t as well as for 64‑bit
    integers. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Utility functions supplied by the build. */
#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

/* Cache of computed terms. */
static seq_t *cache      = NULL;   /* array of found primes */
static idx_t  cache_size = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Allocate or grow the cache to hold at least [min_cap] elements. */
static void ensure_cache_cap(idx_t min_cap)
{
    if ((size_t)min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < (size_t)min_cap) new_cap = (size_t)min_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* unrecoverable failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return true iff 43 is a primitive root modulo the prime p. */
static bool is_primitive_root43(uint64_t p)
{
    if (p <= 1) return false;          /* only p = 2 should pass in practice */
    if (p % 43ULL == 0) return false;   /* base must be coprime with p */

    uint64_t m = p - 1;                /* φ(p) = p-1 for prime p */

    /* Factor m. 64 factors are enough for 64‑bit numbers. */
    uint64_t pf[64];
    uint64_t ex[64];
    size_t   fcnt;
    factor_u64(m, pf, ex, &fcnt);

    /* For every distinct prime factor q of m, check 43^(m/q) mod p != 1. */
    for (size_t i = 0; i < fcnt; ++i)
    {
        seq_t r = mod_pow((seq_t)43, m / pf[i], (seq_t)p);
        if (r == (seq_t)1) return false;
    }
    return true;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1; undefined for n=0 */

    /* Return cached value if already computed. */
    if (n <= cache_size) return cache[n - 1];

    /* Prepare to generate more terms. */
    ensure_cache_cap(n);

    static uint64_t last_checked_prime = 0;   /* 0 means start from 2 */
    uint64_t prime = last_checked_prime ? next_prime(last_checked_prime) : 2;
    idx_t found = cache_size;                 /* number of terms currently known */

    while (found < n)
    {
        if (is_primitive_root43(prime))
        {
            cache[found++] = (seq_t)prime;
        }
        last_checked_prime = prime;
        prime = next_prime(prime);
    }

    cache_size = found;      /* update cache size after additions */
    return cache[n - 1];
}
