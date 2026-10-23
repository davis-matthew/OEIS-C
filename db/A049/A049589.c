/*
 * A049589  Primes p such that x^57 = 2 has a solution mod p.
 * https://oeis.org/A049589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that the congruence x^57 ≡ 2 (mod p) has a solution. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Euclidean algorithm for uint64_t; used to compute gcd(57,p-1). */
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Dynamic cache of primes that satisfy the condition. */
static uint64_t *prime_cache      = NULL;
static size_t   cache_len        = 0;   /* number of cached primes */
static size_t   cache_cap        = 0;   /* allocated capacity */
static uint64_t next_candidate   = 2;   /* next integer to test for primality */

/* Grow the integer cache if necessary. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    uint64_t *new_ptr = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!new_ptr)
        abort();   /* allocation failure */
    prime_cache = new_ptr;
    cache_cap   = new_cap;
}

/* Append a prime to the cache. */
static void append_cache(uint64_t p)
{
    grow_cache(cache_len + 1);
    prime_cache[cache_len++] = p;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {          /* undefined for n=0 – return 0 by convention */
        seq_t zero = 0;
        return zero;
    }

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len) {
        seq_t out = prime_cache[(size_t)n - 1];
        return out;
    }

    /* Generate primes until we have at least n terms. */
    while (cache_len < (size_t)n) {
        if (is_prime(next_candidate)) {
            if (next_candidate == 2) {
                /* 0^57 ≡ 2 (mod 2) – accept p = 2. */
                append_cache(2);
            } else {
                uint64_t p    = next_candidate;
                uint64_t d    = gcd64(57ULL, p - 1ULL);
                uint64_t exp  = (p - 1ULL) / d;
                seq_t   res   = mod_pow(2, exp, p);
                if (res == 1) {
                    append_cache(p);
                }
            }
        }
        ++next_candidate;
    }

    seq_t out = prime_cache[(size_t)n - 1];
    return out;
}
