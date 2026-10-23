/*
 * A141995  Primes congruent to 19 mod 29.
 * https://oeis.org/A141995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 19 (mod 29)
 * Range:     u8 n <= 3, u16 n <= 240, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n-th prime p such that p ≡ 19 (mod 29).
   The function caches previously found terms so that calls with
   arbitrary n are answered in O(n) time overall.  It is safe to call
   the function with n in any order. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; undefined for n = 0 */
        return 0;

    /* static cache of found primes */
    static seq_t *cache = NULL;     /* array of primes (seq_t type)      */
    static size_t cache_len = 0;    /* number of primes cached           */
    static size_t cache_cap = 0;    /* current capacity of cache         */

    /* next candidate that satisfies p ≡ 19 (mod 29) */
    static uint64_t cur_candidate = 19;

    /* If the required prime is already cached, return it. */
    if (cache_len >= (size_t)n)
        return cache[(size_t)(n - 1)];

    /* Grow the cache when necessary.  Use an explicit cast for the
       realloc return value to keep the function C++17 compatible. */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)           /* allocation failure – return 0 as sentinel */
            return 0;
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Generate primes until we have the nth prime. */
    while (cache_len < (size_t)n) {
        if (is_prime((idx_t)cur_candidate))
            cache[cache_len++] = (seq_t)cur_candidate;
        cur_candidate += 29;   /* next number ≡ 19 (mod 29) */
    }

    return cache[(size_t)(n - 1)];
}
