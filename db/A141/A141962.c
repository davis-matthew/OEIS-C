/*
 * A141962  Primes congruent to 22 mod 27.
 * https://oeis.org/A141962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 22 modulo 27
 * Range:     u8 n <= 3, u16 n <= 338, u32 n <= 339, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of primes with residue 22 mod 27 */
    static seq_t *cache = NULL;          /* Dynamically allocated array */
    static size_t cache_size  = 0;       /* Number of elements stored */
    static size_t cache_cap   = 0;       /* Current capacity of the array */

    /* The last value of k such that candidate = 27*k + 22 was examined.
       It starts at 1, so the first candidate considered is 49. */
    static idx_t last_k = 1;

    /* Expand the cache until we have at least n elements. */
    while (cache_size < n)
    {
        /* Candidate number of the form 27*k + 22 */
        seq_t candidate = (seq_t)27 * (seq_t)last_k + 22;
        last_k++;  /* Next candidate will use next k */

        /* If candidate is prime, store it. */
        if (is_prime_big(candidate))
        {
            /* Ensure the cache has enough capacity */
            if (cache_cap <= cache_size)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap * 2);
                void *tmp = realloc(cache, new_cap * sizeof(seq_t));
                cache = (seq_t *)tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
        }
    }

    /* Return the (n-1)-th element (offset 1). */
    return cache[n - 1];
}
