/*
 * A174916  Lesser of twin primes p1 such that p1 + p2^2 - p1^2 is a prime
 *          number.
 * https://oeis.org/A174916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 252, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174916:
   Lesser of twin primes p for which 5*p+4 is also prime.
   The function scans successive integers, tests primality with
   the helper is_prime_big, and caches the result. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cache for the computed terms */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_len = 0;         /* number of cached terms */
static size_t cache_cap = 0;         /* allocated capacity */

/* Next candidate to test (odd integers only). Initialized to 3. */
static idx_t next_candidate = 3;

/* Ensure that cache contains at least n terms (1‑based). */
static void ensure_n_terms(idx_t n)
{
    /* Reserve space for new terms first. */
    while (cache_len < (size_t)n) {
        /* Expand cache if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *new_cache = (seq_t *)realloc(cache,
                                                new_cap * sizeof(seq_t));
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Test the odd candidate. */
        if (is_prime_big((seq_t)next_candidate) &&
            is_prime_big((seq_t)(next_candidate + 2)) &&
            is_prime_big((seq_t)(5 * next_candidate + 4))) {
            cache[cache_len++] = (seq_t)next_candidate;
        }

        /* Move to the next odd number. */
        next_candidate += 2;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* Undefined for n = 0; return 0 as a safeguard. */
        return (seq_t)0;

    ensure_n_terms(n);   /* Fill cache up to n. */
    return cache[(size_t)n - 1];
}
