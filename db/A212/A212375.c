/*
 * A212375  Primes p such that x^18 = 2 has no solution mod p.
 * https://oeis.org/A212375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth prime p such that the congruence
   x^18 = 2 (mod p) has no solution.  The test is performed by
   checking whether 2^{(p-1)/gcd(18,p-1)} ≡ 1 (mod p).  If the
   congruence is not satisfied, then 2 is not an 18th power
   residue modulo p and the prime is accepted.  The implementation
   keeps a static cache of all primes found so far so that any
   n can be answered in O(1) after the cache has been filled.  */

#include <stdint.h>
#include <stdbool.h>

/* utilities used by the program */
#include "../utils/next_prime.h"    /* next_prime(uint64_t) */
#include "../utils/mod_pow.h"       /* mod_pow(seq_t, uint64_t, seq_t) */
#include "../utils/grow_seq.h"      /* grow_seq(seq_t **, size_t *, size_t) */

static seq_t *cache      = NULL;   /* cache of good primes (1‑based) */
static size_t cache_cap = 0;       /* current capacity of cache */
static size_t cache_len = 0;       /* number of primes stored */
static uint64_t last_prime = 2;    /* last prime examined by the sieve */

/* Euclidean algorithm for 64‑bit unsigned integers */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Ensure that cache contains at least n good primes.
   If more primes are needed, generate them one by one, testing the
   18th‑power condition until the desired count is reached. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* find next prime */
        last_prime = next_prime(last_prime);

        /* compute exponent (p-1)/gcd(18,p-1) */
        uint64_t g   = gcd_u64(18, last_prime - 1);
        seq_t   exp  = (seq_t)(last_prime - 1) / (seq_t)g;
        seq_t   mod  = (seq_t)last_prime;
        seq_t   base = (seq_t)2;

        /* check whether 2^{exp} ≡ 1 (mod p) */
        if (mod_pow(base, (uint64_t)exp, mod) != (seq_t)1) {
            /* this prime is required – store it */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len] = (seq_t)last_prime;
            cache_len++;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n=0, but guard anyway */

    ensure_cache(n);
    return cache[n - 1];
}
