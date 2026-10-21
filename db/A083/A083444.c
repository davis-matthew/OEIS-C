/*
 * A083444  Erroneous duplicate of A052034.
 * https://oeis.org/A083444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 52, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   is_prime, is_prime_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083444:
 * a(n) = the n-th prime p such that the sum of the squares of its decimal
 * digits is a prime greater than 2. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_small.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1, but avoid UB */

    /* Static cache for memoization across multiple calls */
    static seq_t *cache         = NULL;   /* array of qualifying primes      */
    static size_t cache_len     = 0;      /* number of elements stored      */
    static size_t cache_cap     = 0;      /* current allocated capacity     */
    static idx_t  last_checked  = 1;      /* last number examined for prime */

    /* Generate and cache primes until we have at least n terms */
    while (cache_len < (size_t)n)
    {
        /* Find next prime number */
        do {
            ++last_checked;
        } while (!is_prime(last_checked));

        /* Compute the sum of the squares of decimal digits of the prime */
        seq_t sum = 0;
        idx_t x   = last_checked;
        while (x > 0) {
            seq_t d     = (seq_t)(x % 10);
            sum += d * d;
            x /= 10;
        }

        /* Verify that the sum is prime and > 2 */
        if (sum > (seq_t)2 && is_prime_small(sum))
        {
            /* Grow the cache array if necessary */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = (cache_cap ? cache_cap * 2 : 16);
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp)   /* allocation failure – abort */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }

            /* Store the qualifying prime */
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
