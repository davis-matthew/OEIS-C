/*
 * A201022  Primes whose multiplicative digital root is 8.
 * https://oeis.org/A201022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   digit_product, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose multiplicative digital root is 8
   (OEIS A201022).  This implementation builds the sequence on
   demand by testing successive primes for the property that the
   repeated product of their decimal digits ends in 8.  The cache
   makes calls in any order efficient.  The algorithm is O(n log n). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"   /* seq_t digit_product(seq_t x); */
#include "../utils/is_prime_small.h"  /* bool is_prime_small(seq_t n); */

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_used = 0;
    static uint64_t next_candidate = 2;  /* next number to test */

    /* grow the cache array as needed */
    if (cache_used + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* extend cache until it contains at least n entries */
    while (cache_used < (size_t)n) {
        /* find next prime whose digit product root is 8 */
        while (1) {
            if (is_prime_small(next_candidate)) {
                seq_t x = (seq_t)next_candidate;
                while (x >= 10)          /* reduce until single digit */
                    x = digit_product(x);
                if (x == 8)
                    break;              /* we found a qualifying prime */
            }
            ++next_candidate;
        }

        /* store the prime */
        cache[cache_used++] = (seq_t)next_candidate;
        ++next_candidate;   /* move to the next candidate for future calls */
    }

    return cache[n - 1];
}
