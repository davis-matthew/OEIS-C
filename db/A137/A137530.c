/*
 * A137530  Primes of the form 5k^2 + 1.
 * https://oeis.org/A137530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 7, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes of the form 5k²+1 where k=6j (so p=180j²+1).  The
   algorithm generates successive candidates j = 1, 2, …, tests them
   for primality with is_prime_big, and memoises the found primes so that
   calls may be made in any order.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Memoisation state */
    static seq_t *cache = NULL;        /* array of found primes */
    static size_t cache_cap = 0;       /* allocated capacity */
    static size_t cache_len = 0;       /* number of primes stored */
    static idx_t next_j = 1;           /* next j to test */

    if (n == 0u) return 0; /* undefined for n<1, harmless */

    /* Grow cache to hold at least n entries */
    if (cache_len < (size_t)n) {
        /* Allocate or grow the cache */
        size_t needed = n;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < needed) new_cap = needed;
            /* cast required for C++ */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1); /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate primes until we have n of them */
        while (cache_len < (size_t)n) {
            /* candidate = 180 * j * j + 1 */
            seq_t candidate = (seq_t)180 * (seq_t)next_j * (seq_t)next_j + 1;

            if (is_prime_big(candidate))
                cache[cache_len++] = candidate;

            ++next_j;
        }
    }

    return cache[(size_t)n - 1];
}
