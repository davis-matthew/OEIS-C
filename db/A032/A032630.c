/*
 * A032630  Primes that are concatenations of n with n + 7.
 * https://oeis.org/A032630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 13, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for each n (starting at 1) form the decimal concatenation
   of n and n+7, test whether that number is prime; the sequence a(n) is
   the nth prime obtained this way.  The algorithm memoises results,
   so calls may be in any order. */
#include <stdlib.h>
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL; /* array of found primes */
    static idx_t cache_size  = 0;    /* allocated capacity */
    static idx_t primes_found = 0;   /* number of primes stored */
    static idx_t next_n = 1;         /* next n to test */

    /* extend cache until we have at least n primes */
    while (primes_found < n) {
        seq_t cand = concat_parts(next_n, next_n + 7);

        if (is_prime_big(cand)) {
            /* grow cache if needed */
            if (primes_found == cache_size) {
                size_t new_size = cache_size ? cache_size * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
                cache = tmp;
                cache_size = new_size;
            }
            cache[primes_found++] = cand;
        }
        next_n++;
    }

    return cache[n - 1];
}
