/*
 * A040079  Primes p such that x^3 = 17 has no solution mod p.
 * https://oeis.org/A040079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040079 – primes p for which x^3 ≡ 17 (mod p) has no solution.
   The sequence is obtained by testing primes p ≡ 1 (mod 3) and checking
   whether 17^((p−1)/3) mod p ≠ 1.  The cache below stores all found
   primes so that repeated calls are fast. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isPrimeTrial.h"
#include "../utils/mod_pow.h"

/* Cached primes.  The array is expanded on demand. */
static seq_t *prime_cache = NULL;
static size_t cache_size   = 0;
static size_t cache_cap    = 0;

/* Last candidate examined; starts at the first odd prime belonging to the
   sequence (p = 7).  It is advanced only when more terms are required. */
static uint64_t next_candidate = 7;

seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n <= cache_size) {
        return prime_cache[n - 1];
    }

    /* Allocate initial cache if needed. */
    if (cache_cap == 0) {
        cache_cap   = 16;
        prime_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!prime_cache) abort();
    }

    /* Continue searching from the last examined candidate until we have
       the required number of terms. */
    while (cache_size < (size_t)n) {
        /* Only candidates ≡ 1 (mod 3) can belong to the sequence. */
        if ((next_candidate % 3) == 1 && isPrimeTrial((seq_t)next_candidate)) {
            /* Test whether 17 is a cubic residue modulo this prime. */
            uint64_t exp = (next_candidate - 1) / 3;
            seq_t res    = mod_pow((seq_t)17, exp, (seq_t)next_candidate);
            if (res != (seq_t)1) {
                /* Store the prime, expanding the cache if necessary. */
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp     = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    prime_cache = tmp;
                    cache_cap   = new_cap;
                }
                prime_cache[cache_size++] = (seq_t)next_candidate;
            }
        }
        /* Advance to the next odd number. */
        next_candidate += 2;
    }

    return prime_cache[n - 1];
}
