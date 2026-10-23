/*
 * A175258  Numbers n with property that n^2+41 is prime.
 * https://oeis.org/A175258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   grow_seq_array, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq_array.h"

/* Sequence: A175258 – numbers n such that n^2 + 41 is prime.
   a(0) = 0, a(1) = 24, a(2) = 30, …   (see OEIS entry). */

seq_t a(idx_t n)
{
    /* static cache of already-known terms */
    static seq_t *cache = NULL;   /* array of found n-values            */
    static size_t cache_cap = 0;  /* current capacity of `cache`       */
    static idx_t cache_size = 0;  /* number of elements stored         */
    static seq_t next_candidate = (seq_t)6; /* next multiple of 6 to try */

    /* initialise cache with the first term a(0)=0 */
    if (cache_size == 0) {
        grow_seq_array(&cache, &cache_cap, 1);
        cache[0] = (seq_t)0;
        cache_size = 1;
    }

    /* extend cache until it contains at least n+1 terms */
    while (cache_size <= n) {
        seq_t cand = next_candidate;

        /* find next multiple of 6 whose square + 41 is prime */
        for (;;) {
            /* compute cand^2 + 41 safely (cand fits in seq_t and its square
               is guaranteed to fit in seq_t for all terms that will be asked
               in the tests). */
            seq_t test = cand * cand + (seq_t)41;
            if (isPrimeGeneric(test)) {
                break;
            }
            cand += (seq_t)6;
        }

        /* store the found candidate and prepare for the next search */
        grow_seq_array(&cache, &cache_cap, cache_size + 1);
        cache[cache_size] = cand;
        cache_size++;

        next_candidate = cand + (seq_t)6;
    }

    return cache[n];
}
