/*
 * A105906  Primes for which -34 is a primitive root.
 * https://oeis.org/A105906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, push_seq, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes OEIS A105906.
   A prime p is accepted if -34 is a primitive root modulo p.
   For each prime p > 2, 34 must be coprime to p (i.e. p ≠ 17),
   and for every prime divisor q of p‑1 we require
   g^((p‑1)/q) ≠ 1 (mod p) where g = (-34 mod p).  The function keeps
   a static cache of the primes found so far and expands it as needed. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/simple_sieve.h"
#include "../utils/push_seq.h"
#include "../utils/mod_pow.h"

static bool is_neg34_primitive_root(uint32_t p, const uint32_t *prime_list,
                                    size_t prime_count)
{
    if (p <= 2 || p == 17) return false;          /* 34 ≡ 0 (mod p) for p = 17 */

    uint32_t rem = 34 % p;                        /* 0 <= rem < p */
    uint32_t g   = (p - rem) % p;                 /* g = (-34 mod p) */

    uint64_t pp1 = (uint64_t)p - 1;
    uint64_t temp = pp1;

    for (size_t i = 0; i < prime_count &&
         (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= temp; ++i)
    {
        uint32_t q = prime_list[i];
        if (temp % q == 0)
        {
            if (mod_pow((uint64_t)g, (pp1 / q), (uint64_t)p) == 1) return false;
            while (temp % q == 0) temp /= q;
        }
    }

    if (temp > 1)
    {
        uint32_t q = (uint32_t)temp;
        if (mod_pow((uint64_t)g, (pp1 / q), (uint64_t)p) == 1) return false;
    }

    return true;
}

seq_t a(idx_t n)
{
    static uint32_t *prime_list = NULL;
    static size_t   prime_count = 0;
    static size_t   next_prime_idx = 0;
    static seq_t *  cache = NULL;
    static idx_t    cache_cap = 0;
    static idx_t    cache_size = 0;
    static bool     initialized = false;

    if (!initialized)
    {
        const uint32_t LIMIT = 2000000U;
        simple_sieve(LIMIT, &prime_list, &prime_count);
        initialized = true;
    }

    while (cache_size < n)
    {
        /* Find next qualifying prime. */
        for (; next_prime_idx < prime_count; ++next_prime_idx)
        {
            uint32_t p = prime_list[next_prime_idx];
            if (is_neg34_primitive_root(p, prime_list, prime_count))
            {
                push_seq(&cache, &cache_cap, &cache_size, (seq_t)p);
                ++next_prime_idx;
                break;
            }
        }

        /* If we ran out of primes in the current sieve, enlarge it. */
        if (next_prime_idx >= prime_count)
        {
            uint32_t old_limit = prime_count;                     /* approx limit */
            uint32_t new_limit = old_limit * 2;
            uint32_t *new_list = NULL;
            size_t new_count = 0;
            simple_sieve(new_limit, &new_list, &new_count);
            free(prime_list);
            prime_list = new_list;
            prime_count = new_count;
            next_prime_idx = 0;                                    /* restart scan */
        }
    }

    return cache[n - (idx_t)1];
}
