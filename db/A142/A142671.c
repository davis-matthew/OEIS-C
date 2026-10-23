/*
 * A142671  Primes congruent to 10 mod 57.
 * https://oeis.org/A142671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th prime congruent to 10 (mod 57) */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* out of range, return 0 */
        return 0;

    /* static cache of primes found so far */
    static idx_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t last = 0;     /* last integer examined */

    /* generate primes until we have at least n entries in the cache */
    while (cache_len < (size_t)n) {
        ++last;                                       /* next candidate */
        if (last % 57 != 10)                          /* skip non‐congruents */
            continue;

        if (is_prime(last)) {                          /* prime test */
            /* grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                idx_t *tmp = (idx_t *)realloc(cache,
                                               new_cap * sizeof(idx_t));
                if (!tmp)                /* allocation failure – abort */
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last;                 /* store the prime */
        }
    }
    return (seq_t)cache[n - 1];                         /* return the nth prime */
}
