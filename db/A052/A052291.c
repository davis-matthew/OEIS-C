/*
 * A052291  Primes p such that 4p^2 + 1 is also prime.
 * https://oeis.org/A052291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = primes p such that 4*p*p + 1 is also prime. The function
 *            searches the primes in ascending order, remembers the ones that
 *            satisfy the criterion, and returns the n‑th such prime
 * Range:     u8 n <= 14, u16 n <= 697, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    /* allow n = 0; behaviour below the offset is undefined */
    if (n == 0) return (seq_t)0;

    /* static cache of already found terms */
    static seq_t *cache = NULL;      /* array of found primes */
    static size_t cache_cap = 0;     /* current allocation capacity */
    static size_t cache_len = 0;     /* number of primes stored so far */
    static uint64_t next_candidate = 2;   /* next integer to test */

    size_t target = (size_t)n;

    /* If we already have the requested term, return it. */
    if (cache_len >= target)
        return cache[target - 1];

    /* Otherwise extend the cache until we have at least n terms. */
    while (cache_len < target) {
        uint64_t p = next_candidate;

        /* test whether p is prime */
        if (is_prime_u64(p)) {
            uint64_t val = 4ULL * p * p + 1ULL;   /* 4p^2 + 1 */
            if (is_prime_u64(val)) {
                /* enlarge the array if necessary */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    /* realloc returns void*; cast for C++ */
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;   /* allocation failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len] = (seq_t)p;
                ++cache_len;
            }
        }

        /* move on to the next integer */
        ++next_candidate;
    }

    return cache[target - 1];
}
