/*
 * A019372  Primes with primitive root 47.
 * https://oeis.org/A019372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, factorize, mod_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime p (n ≥ 1) such that 47 is a primitive
 * root modulo p.  This implementation generates primes up to a
 * fixed limit, tests each for the primitive‑root property, and
 * stores found primes in a static cache for fast repeated calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/factorize.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_realloc.h"

/* Return true if 47 is a primitive root modulo prime p (p > 2).  */
static bool isPrimitiveRoot47(uint64_t p)
{
    if (p == 2)            /* 47 ≡ 1 (mod 2) and φ(2)=1, so 47 is primitive root of 2 */
        return true;

    uint64_t phi = p - 1;
    uint64_t pf[64];       /* distinct prime factors of φ */
    int    exps[64];       /* their exponents (unused) */
    size_t nf = factorize(phi, pf, exps);

    for (size_t i = 0; i < nf; ++i) {
        uint64_t q   = pf[i];
        uint64_t exp = phi / q;
        if (mod_pow((seq_t)47, exp, (seq_t)p) == (seq_t)1)
            return false;   /* fails the primitivity test */
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* --------- Static state ------------------------------------------ */
    static uint64_t *prime_list = NULL;   /* primes up to current limit      */
    static size_t   prime_count = 0;      /* number of primes fetched          */
    static size_t   prime_index = 0;      /* next prime to examine            */

    static seq_t *value_cache = NULL;     /* cached primitive‑root primes    */
    static size_t cache_len   = 0;        /* number of cached entries        */
    static size_t cache_cap   = 0;        /* capacity of cache array         */

    /* --------- Initialise prime list on first call -------------------- */
    if (!prime_list) {
        /* Rough upper bound: 5 million covers n ≤ 10 000 comfortably */
        idx_t limit = 5000000;
        build_primes_upto(limit, &prime_list, &prime_count);
        prime_index = 0;
    }

    /* --------- Expand cache until we have n terms ---------------------- */
    while (cache_len < n) {
        /* Ensure we have a prime to test */
        if (prime_index >= prime_count) {
            /* Need more primes: extend the sieve region */
            idx_t new_limit = (idx_t)(prime_count * 2 + 1);
            free(prime_list);
            build_primes_upto(new_limit, &prime_list, &prime_count);
            prime_index = 0;
        }

        uint64_t p = prime_list[prime_index++];

        /* Skip 47 itself (it is not included in the sequence) */
        if (p == 47)
            continue;

        if (isPrimitiveRoot47(p)) {
            if (cache_len == cache_cap) {   /* grow cache */
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                value_cache = seq_realloc(value_cache, cache_cap, new_cap);
                cache_cap   = new_cap;
            }
            value_cache[cache_len++] = (seq_t)p;
        }
    }

    return value_cache[n - 1];
}
