/*
 * A059236  Primes p such that x^41 = 2 has no solution mod p.
 * https://oeis.org/A059236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 159, u32 n <= 9575, u64 n <= 9575,
 *            u128 n <= 9575, big any n
 * Helpers:   mod_pow, next_prime, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the primes p for which the congruence
   x^41 ≡ 2 (mod p) has no solution.  It collects primes p ≡ 1 (mod 41)
   and tests whether 2^((p−1)/41) ≠ 1 (mod p).  The routine searches
   primes on demand and memoises the results so that arbitrary order of
   calls is supported. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_alloc.h"

/* Ensure that the cache array can hold at least `needed` elements. */
static void ensure_cap(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed > *cap) {
        size_t newcap = (*cap == 0) ? 4 : *cap << 1;
        while (newcap < needed) {
            newcap <<= 1;
        }
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* Cache of sequence values (index 0 corresponds to a(1)). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, harmless */

    /* Expand the cache until it contains at least n elements. */
    while (cache_len < (size_t)n) {
        static uint64_t start = 2;          /* start searching from 2   */
        uint64_t p = next_prime(start);     /* next prime ≥ start      */
        start = p + 1;                      /* next search starts after p */

        /* Only primes congruent to 1 mod 41 can fail the test. */
        if (p % 41ULL == 1ULL) {
            uint64_t exp = (p - 1ULL) / 41ULL;
            seq_t base = (seq_t)2;
            seq_t mod  = (seq_t)p;
            seq_t val  = mod_pow(base, exp, mod);
            if (val != (seq_t)1) {
                ensure_cap(cache_len + 1, &cache_cap, &cache);
                cache[cache_len++] = (seq_t)p;
            }
        }
    }

    return cache[n - 1];
}
