/*
 * A142681  Primes congruent to 26 mod 57.
 * https://oeis.org/A142681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 188, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes congruent to 26 mod 57 */
/* a(n) is the nth prime p such that p ≡ 26 (mod 57). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0; /* undefined for n < 1 */
    }

    /* static cache of previously found primes */
    static seq_t *cache      = NULL;
    static size_t cache_len = 0;   /* number of stored primes */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t  cand       = 26; /* last candidate examined */

    /* extend cache until it contains at least n primes */
    while (cache_len < n) {
        cand += 57; /* next number congruent to 26 modulo 57 */

        if (is_prime(cand)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? 2 * cache_cap : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    cache = NULL;
                    cache_len = 0;
                    cache_cap = 0;
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            /* store the found prime */
            cache[cache_len++] = (seq_t)cand;
        }
    }

    return cache[n - 1]; /* a(1) is the first element */
}
