/*
 * A141951  Primes congruent to 5 mod 27.
 * https://oeis.org/A141951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 357, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n-th prime ≡ 5 (mod 27) by testing candidates of that form
   and caching found primes. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* Static cache of primes ≡ 5 mod 27 */
    static seq_t *cache       = NULL;
    static size_t   cache_cap = 0;
    static size_t   cache_len = 0;

    /* Next candidate of the form 27k + 5 to test for primality */
    static idx_t next_candidate = 5;

    /* Bail if called with n = 0 (offset is 1) */
    if (n == 0)
        return (seq_t)0;

    /* Ensure we have at least n primes cached */
    while (cache_len < n) {
        /* Test the current candidate for primality */
        if (is_prime(next_candidate)) {
            /* Grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {            /* allocation failure → return zero */
                    return (seq_t)0;
                }
                cache      = tmp;
                cache_cap  = new_cap;
            }
            /* Store the found prime */
            cache[cache_len++] = (seq_t)next_candidate;
        }
        /* Advance to the next number of the form 27k + 5 */
        next_candidate += 27;
    }

    /* Return the n-th prime (1‑based index) */
    return cache[n - 1];
}
