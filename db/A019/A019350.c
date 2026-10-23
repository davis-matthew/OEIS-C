/*
 * A019350  Primes with primitive root 23.
 * https://oeis.org/A019350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes for which 23 is a primitive root – OEIS A019350 */
/* This implementation searches primes and tests the primitive‑root
   condition by factorising p‑1 with trial division.  Results are cached
   so that calls in any order remain O(n) in total. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/is_prime.h"

static uint64_t _smallest_prime_factor(uint64_t n)
{
    if (n % 2 == 0) return 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return d;
    }
    return n;               /* n itself is prime */
}

static uint64_t _mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1) result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

static bool _has_primitive_root_23(uint64_t p)
{
    if (p == 23) return false;          /* 23 itself does not qualify */

    uint64_t pm1 = p - 1;
    uint64_t tmp = pm1;
    const uint64_t base = 23;

    while (tmp > 1) {
        uint64_t q = _smallest_prime_factor(tmp);
        uint64_t exp = pm1 / q;
        if (_mod_pow(base, exp, p) == 1) return false;

        while (tmp % q == 0) tmp /= q;
    }
    return true;
}

static seq_t *cache = NULL;     /* cached sequence values */
static idx_t  ncache = 0;       /* number of values cached */
static idx_t  start  = 2;       /* next integer to test for primality */

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms stored. */
    while (ncache < n) {
        /* Find the next prime candidate. */
        while (!is_prime(start)) ++start;

        /* Check the primitive‑root condition. */
        if (_has_primitive_root_23((uint64_t)start)) {
            /* Grow the cache array if necessary. */
            if (ncache == 0 || ncache % 128 == 0) {
                size_t newcap = (ncache == 0) ? 128 : ncache * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[ncache++] = (seq_t)start;
        }
        ++start; /* advance to next candidate */
    }

    return cache[n - 1];
}
