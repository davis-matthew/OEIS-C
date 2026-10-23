/*
 * A142503  Primes congruent to 44 mod 51.
 * https://oeis.org/A142503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 208, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 44 modulo 51.
 * The sequence starts with a(1) = 197.  The algorithm
 * generates successive integers ≡ 44 (mod 51) and
 * keeps those that are prime.  For efficiency the
 * results are cached so that subsequent calls with
 * larger or smaller n are answered in O(1).
 * The routine is fully static and works regardless of
 * whether seq_t is a built‑in integer type or an
 * arbitrary‑precision integer class (boost::multiprecision::cpp_int).
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Ensure that the utility for primality testing is available. */
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n) {
    /* static cache of found primes */
    static seq_t *cache = NULL;          /* array of (n) primes */
    static size_t cache_size = 0;        /* number of primes stored */
    static size_t cache_cap   = 0;        /* capacity of cache */
    static idx_t next_candidate = 44;    /* next number to test */

    if (n == 0) return 0;               /* safety for offset<1 */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) abort();    /* allocation failure */
        cache_size = 0;
        next_candidate = 44;
    }

    /* Extend cache until we have at least n primes */
    while (cache_size < n) {
        /* Advance to next number in the residue class 44 mod 51 */
        next_candidate += 51;
        if (next_candidate < 44)      /* overflow guard */
            break;

        if (is_prime(next_candidate)) {
            /* Grow cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               sizeof(seq_t) * new_cap);
                if (tmp == NULL) abort(); /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
    }

    return cache[n-1];
}
