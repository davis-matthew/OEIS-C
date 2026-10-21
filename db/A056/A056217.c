/*
 * A056217  Primes p for which the period of reciprocal 1/p is (p-1)/12.
 * https://oeis.org/A056217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime p such that the multiplicative order of 10
 *            modulo p equals (p‑1)/12. (The decimal period of 1/p is
 *            (p‑1)/12.)
 * Range:     u8 n <= 1, u16 n <= 72, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t) */
#include "../utils/mod_pow.h"       /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */

/* Check whether p (a prime) satisfies ord_10(p) = (p-1)/12. */
static bool has_order_twelve(seq_t p)
{
    idx_t t = ((idx_t)p - 1) / 12;        /* t = (p-1)/12 */

    /* First, 10^t must be 1 mod p. */
    if (mod_pow((seq_t)10, (uint64_t)t, p) != (seq_t)1)
        return false;

    /* Ensure that no proper divisor of t yields 1. */
    idx_t temp = t;
    for (idx_t q = 2; q * q <= temp; ++q) {
        if (temp % q == 0) {
            /* Divide out all copies of q. */
            while (temp % q == 0)
                temp /= q;
            /* If 10^(t/q) == 1, then order < t. */
            if (mod_pow((seq_t)10, (uint64_t)(t / q), p) == (seq_t)1)
                return false;
        }
    }
    /* Remaining prime factor. */
    if (temp > 1) {
        if (mod_pow((seq_t)10, (uint64_t)(t / temp), p) == (seq_t)1)
            return false;
    }
    return true;
}

/* Static cache for memoisation of found primes. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_candidate = 13;      /* first number ≡ 1 (mod 12) after 12 */

/* Main function defined per specification. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 0; sequence starts at n=1 */

    /* Grow cache until it contains n terms. */
    while (cache_len < n) {
        /* Find next prime of the form 12k+1 that satisfies the order condition. */
        while (true) {
            if (is_prime((idx_t)next_candidate)) {
                if (has_order_twelve((seq_t)next_candidate))
                    break;   /* Valid prime found. */
            }
            next_candidate += 12;   /* Skip to next candidate ≡ 1 (mod 12). */
        }

        /* Append the found prime to the cache. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {              /* allocation failed – clear state and bail */
                free(cache);
                cache = NULL;
                cache_cap = 0;
                cache_len = 0;
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)next_candidate;

        /* Advance to the next candidate for the next search. */
        next_candidate += 12;
    }

    return cache[n - 1];   /* Offset is 1, so return the (n-1)‑th cached value. */
}
