/*
 * A049592  Primes p such that x^60 = 2 has a solution mod p.
 * https://oeis.org/A049592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that the congruence x^60 ≡ 2 (mod p) has a solution. */
/* A number p satisfies this iff 2^{(p‑1)/gcd(60,p‑1)} ≡ 1 (mod p).     */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: greatest common divisor for 64‑bit unsigned integers. */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Prime test function (provided by the framework). */
#include "../utils/is_prime.h"

/* Modular exponentiation function (provided by the framework). */
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* A cache of primes that satisfy the condition. */
    static seq_t *cache = NULL;      /* array of primes */
    static size_t cache_len = 0;     /* number of cached primes */
    static size_t cache_cap = 0;     /* allocated capacity */
    static uint64_t next_candidate = 3; /* next odd number to test (2 is handled separately) */

    /* Initialise the cache with the first term (p = 2). */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* handle allocation failure */
        cache[0] = (seq_t)2;            /* p = 2 satisfies 0^60 ≡ 2 (mod 2) */
        cache_len = 1;
    }

    /* Generate additional primes until we have at least n primes in the cache. */
    while (cache_len < n) {
        /* Check primality of the current candidate. */
        if (is_prime((idx_t)next_candidate)) {
            uint64_t p = next_candidate;
            uint64_t d = gcd_u64(p - 1, 60ULL);
            uint64_t exp = (p - 1) / d;
            seq_t res = mod_pow((seq_t)2, exp, (seq_t)p);

            if (res == (seq_t)1) { /* condition satisfied */
                /* Grow cache if necessary. */
                if (cache_len == cache_cap) {
                    cache_cap = (cache_cap == 0 ? 8 : cache_cap * 2);
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) return 0; /* handle allocation failure */
                    cache = tmp;
                }
                cache[cache_len++] = (seq_t)p;
            }
        }
        /* Advance to the next odd candidate. */
        next_candidate += 2;
    }

    return cache[n - 1];
}
