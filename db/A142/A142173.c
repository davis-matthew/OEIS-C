/*
 * A142173  Primes congruent to 23 mod 39.
 * https://oeis.org/A142173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 271, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes congruent to 23 mod 39: a(n) = the nth prime p with p ≡ 23 (mod 39) (a(1)=23) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; for safety return 0 if called with 0 */
        return 0;

    /* static cache for previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_sz   = 0;  /* number of elements stored */
    static size_t cache_cap  = 0;  /* current capacity of cache */
    static seq_t next_candidate = 23;  /* first number to test */

    /* return cached value if already known */
    if (n <= cache_sz)
        return cache[n - 1];

    /* lazy allocation of cache */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;          /* allocation failure – unlikely in this use case */
    }

    /* generate primes until we have at least n entries */
    while (cache_sz < n) {
        /* test current candidate */
        if (is_prime((idx_t)next_candidate)) {
            /* grow cache if needed */
            if (cache_sz >= cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache); cache = NULL;
                    return 0;  /* allocation failure */
                }
                cache = tmp;
            }
            cache[cache_sz++] = next_candidate;  /* store new term */
        }
        next_candidate += 39;   /* jump to next number ≡ 23 (mod 39) */
    }

    return cache[n - 1];
}
