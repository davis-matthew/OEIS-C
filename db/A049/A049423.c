/*
 * A049423  Primes of the form k^2 + 3.
 * https://oeis.org/A049423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 3708, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime of the form k² + 3.
   For n = 1 the result is 3, for n = 2 it is 7, etc.
   The implementation lazily generates candidates, tests primality
   with isPrimeGeneric, and caches found primes so that calls
   in any order remain fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Lazy static cache of found primes */
    static seq_t *cache      = NULL; /* array of primes */
    static size_t cache_count = 0;    /* number of elements stored */
    static size_t cache_cap   = 0;    /* allocated capacity of cache */
    static idx_t last_k       = 0;    /* next k to test (starting at 0) */

    if (n == 0) {
        return (seq_t)0;                /* undefined for n < 1 */
    }

    /* Grow cache until it contains at least n terms. */
    while (cache_count < n) {
        seq_t candidate = (seq_t)last_k * (seq_t)last_k + (seq_t)3;

        if (isPrimeGeneric(candidate)) {
            /* Ensure there is room for another element. */
            if (cache_count >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2U : 128U;
                seq_t *tmp = alloc_seq_array(new_cap);
                if (!tmp) {
                    /* Allocation failed – abort. */
                    exit(EXIT_FAILURE);
                }
                /* Copy existing elements to the new array. */
                for (size_t i = 0; i < cache_count; ++i) {
                    tmp[i] = cache[i];
                }
                /* Free old array and update pointers/capacity. */
                if (cache) {
                    free_seq_array(cache);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the new prime. */
            cache[cache_count++] = candidate;
        }
        ++last_k;
    }

    return cache[n - 1];
}
