/*
 * A019361  Primes with primitive root 35.
 * https://oeis.org/A019361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that 35 is a primitive root modulo p.
 *            Implementation: iterate over primes and test the primitive‑root
 *            condition via modular exponentiation
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* Test whether a given prime p has 35 as a primitive root modulo p.
   The function assumes that prime_list contains all primes up to sqrt(p).
   It verifies that 35^((p-1)/q) ≢ 1 (mod p) for every distinct prime
   divisor q of p−1. */
static bool has_primitive_root_35(uint64_t p, const uint64_t *prime_list, size_t prime_len)
{
    if (p == 2 || p == 3)               /* 35 is a primitive root mod 2 and 3 */
        return true;
    if (p % 5 == 0 || p % 7 == 0)       /* 35 ≡ 0 (mod p) → never primitive */
        return false;

    uint64_t phi = p - 1;
    uint64_t tmp = phi;

    for (size_t i = 0; i < prime_len && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t q = prime_list[i];
        if (tmp % q == 0) {
            uint64_t exp = phi / q;
            if (mod_pow((seq_t)35, exp, (seq_t)p) == (seq_t)1)
                return false;
            while (tmp % q == 0)
                tmp /= q;
        }
    }
    if (tmp > 1) {                       /* remaining prime factor > sqrt(phi) */
        uint64_t exp = phi / tmp;
        if (mod_pow((seq_t)35, exp, (seq_t)p) == (seq_t)1)
            return false;
    }
    return true;
}

/* Main sequence function.  It keeps static caches of the terms
   found so far and of the list of known primes.  Calls may arrive
   in any order; the function expands its caches until it has at
   least n terms. */
seq_t a(idx_t n)
{
    /* ---------- static state ---------- */
    static seq_t *cache       = NULL;  /* cache of sequence terms */
    static size_t  cache_cap  = 0;
    static size_t  cache_len  = 0;

    static uint64_t *primes    = NULL; /* list of known primes */
    static size_t   primes_cap = 0;
    static size_t   primes_len = 0;

    static uint64_t last_checked = 1;   /* last candidate tested */

    if (n == 0)
        return (seq_t)0;               /* undefined for offset 0, but safe */

    /* Expand caches if needed */
    if (cache_len < (size_t)n) {
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < (size_t)n)
                new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        if (primes_cap == 0) {
            primes_cap = 64;
            primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
            if (!primes)
                abort();
        }

        /* Find further terms until we have at least n of them */
        while (cache_len < (size_t)n) {
            ++last_checked;

            /* Simple primality test using the primes we already know */
            bool is_prime = true;
            if (last_checked < 2) {
                is_prime = false;
            } else if (last_checked == 2) {
                is_prime = true;
            } else if (last_checked % 2 == 0) {
                is_prime = false;
            } else {
                for (size_t i = 0; i < primes_len; ++i) {
                    uint64_t p = primes[i];
                    if (p * p > last_checked)
                        break;
                    if (last_checked % p == 0) {
                        is_prime = false;
                        break;
                    }
                }
            }
            if (!is_prime)
                continue;

            /* Store the new prime */
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp)
                    abort();
                primes = tmp;
            }
            primes[primes_len++] = last_checked;

            /* Test 35 as primitive root */
            if (has_primitive_root_35(last_checked, primes, primes_len)) {
                cache[cache_len++] = (seq_t)last_checked;
            }
        }
    }

    return cache[(size_t)n - 1];
}
