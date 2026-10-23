/*
 * A142792  Primes congruent to 31 mod 60.
 * https://oeis.org/A142792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 412, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142792: primes ≡ 31 (mod 60).
   Computation is by brute‑force testing of each candidate in that residue
   class with is_prime_small(), storing found primes in a static cache so
   that arbitrary calls are O(1) after the first run. */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */
#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdint.h>   /* uint64_t */

#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t n); */

/* Return the n‑th prime congruent to 31 mod 60 (a(1)=31). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* offset starts at 1 */
        return (seq_t)0;

    /* static cache of primes in this residue class */
    static seq_t *cache     = NULL;  /* array of found primes       */
    static size_t cache_sz  = 0;     /* number of primes stored     */
    static size_t cache_cap = 0;     /* allocated capacity (elements) */
    static uint64_t next_candidate = 31; /* next candidate ≡31 (mod 60) */

    /* Extend the cache until at least n primes are known. */
    while (cache_sz < (size_t)n) {
        /* Test the current candidate for primality. */
        if (is_prime_small((seq_t)next_candidate)) {
            /* Grow the cache if needed. */
            if (cache_sz + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();          /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz] = (seq_t)next_candidate;
            ++cache_sz;
        }

        /* Advance to the next number ≡31 (mod 60). */
        next_candidate += 60;
    }

    return cache[n - 1];
}
