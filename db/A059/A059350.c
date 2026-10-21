/*
 * A059350  Primes p such that x^38 = 2 has no solution mod p.
 * https://oeis.org/A059350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A059350 is the list of primes p for which
 * there is no solution of x^38 ≡ 2 (mod p).
 * For an odd prime p a solution exists iff
 * 2^((p-1)/gcd(38,p-1)) ≡ 1 (mod p).
 * Thus a(n) is the nth prime such that
 * 2^((p-1)/gcd(38,p-1)) ≠ 1 (mod p). */

/* Standard includes */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_uint64.h"

/* Euclid's algorithm for 64‑bit unsigned integers. */

/* Binary modular exponentiation: computes (base^exp) % mod.
 * Uses 128‑bit intermediate to avoid overflow on 64‑bit types. */
static seq_t mod_pow_seq(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1U)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1U;
    }
    return result;
}

/* Return the nth term of A059350 (offset 1). */
seq_t a(idx_t n)
{
    /* Cache of found terms so far. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* List of primes tested up to the current point. */
    static uint64_t *primes = NULL;
    static size_t primes_len = 0, primes_cap = 0;

    /* Highest integer we've examined for primality. */
    static uint64_t last_tested = 1;

    /* Initialise prime list with the first prime 2. */
    if (primes_len == 0) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_len = 1;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Find the next prime. */
        uint64_t p = 0;
        for (;;) {
            ++last_tested;
            if (last_tested == 2) {          /* 2 is the only even prime */
                p = 2;
                break;
            }
            if ((last_tested & 1U) == 0)     /* skip even numbers */
                continue;

            bool is_prime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                uint64_t q = primes[i];
                if (q * q > last_tested)
                    break;
                if (last_tested % q == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                if (primes_len == primes_cap) {
                    primes_cap = primes_cap ? primes_cap * 2 : 16;
                    primes = (uint64_t *)realloc(primes,
                                                 primes_cap * sizeof(uint64_t));
                }
                primes[primes_len++] = last_tested;
                p = last_tested;
                break;
            }
        }

        /* The prime 2 never satisfies the required condition. */
        if (p == 2)
            continue;

        /* Check whether 2 is a 38‑th power residue modulo p. */
        uint64_t g   = gcd_uint64(38ULL, p - 1);
        uint64_t exp = (p - 1) / g;
        seq_t residue = mod_pow_seq((seq_t)2, exp, (seq_t)p);

        if (residue != (seq_t)1) {           /* 2 is not a residue */
            /* Append to our cache. */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache,
                                         cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
