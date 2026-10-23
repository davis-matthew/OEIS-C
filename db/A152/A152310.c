/*
 * A152310  Primes p such that the multiplicative order of 2 modulo p is
 *          (p-1)/10.
 * https://oeis.org/A152310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 79, u32 n <= 38, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that ord_p(2) = (p-1)/10 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"

/* Check whether a prime p satisfies ord_p(2) = (p-1)/10.
   p is assumed to be 1 (mod 10). */
static bool has_tenth_order_prime(seq_t p)
{
    /* (p-1)/10 fits within uint64_t for the ranges we consider. */
    uint64_t m = (uint64_t)((p - (seq_t)1) / (seq_t)10);

    /* 2^m ≡ 1 (mod p) is necessary. */
    if (mod_pow((seq_t)2, m, p) != (seq_t)1)
        return false;

    /* Test each distinct prime factor q of m.
       If 2^(m/q) ≡ 1 (mod p), the order would divide m/q, not m. */
    uint64_t tmp = m;
    for (uint64_t q = 2; q * q <= tmp; ++q) {
        if (tmp % q == 0) {
            /* q is a prime factor of m. */
            if (mod_pow((seq_t)2, m / q, p) == (seq_t)1)
                return false;
            /* Skip all powers of q. */
            while (tmp % q == 0)
                tmp /= q;
        }
    }
    if (tmp > 1) {                /* remaining prime factor > sqrt(m) */
        if (mod_pow((seq_t)2, m / tmp, p) == (seq_t)1)
            return false;
    }
    return true;
}

/* Return the n-th prime satisfying the order condition. */
seq_t a(idx_t n)
{
    /* Static cache of found primes. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms          */
    static size_t cache_cap = 0;   /* capacity of the allocation       */

    /* Grow the cache until it contains at least n terms. */
    if (cache_len < (size_t)n) {
        /* Ensure cache has some non‑zero capacity to start with. */
        if (cache_cap == 0) {
            cache_cap = 16;
            cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        }

        /* Current candidate.  We step by 10 to stay 1 (mod 10). */
        seq_t candidate = 151;
        if (cache_len > 0)
            candidate = cache[cache_len - 1] + (seq_t)10;

        /* Continue searching until we have enough terms. */
        while (cache_len < (size_t)n) {
            /* Test primality. */
            if (is_prime_big(candidate)) {
                /* Candidate is prime; now test the order condition. */
                if (has_tenth_order_prime(candidate)) {
                    /* Grow cache if necessary. */
                    if (cache_len == cache_cap) {
                        cache_cap *= 2;
                        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_len++] = candidate;
                }
            }
            /* Next candidate congruent to 1 modulo 10. */
            candidate += (seq_t)10;
        }
    }
    return cache[(size_t)n - 1];
}
