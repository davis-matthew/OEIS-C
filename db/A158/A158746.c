/*
 * A158746  Numbers n such that 30*n + 13 is prime.
 * https://oeis.org/A158746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: linear scan – for each non‑negative integer n, test whether
   30*n+13 is prime.  Store the qualifying n’s in a cache and return
   the n‑th element of that cache.  The function works for both
   unsigned integer and arbitrary‑precision seq_t types. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache for previously computed elements */
    static seq_t *cache = NULL;          /* array of found n's          */
    static size_t cache_size = 0;        /* number of elements cached   */
    static size_t cache_cap  = 0;        /* capacity of the cache array */
    static idx_t next_candidate = 0;     /* next n to test for primality */

    /* Guard against invalid indices (not expected per the spec). */
    if (n == 0) return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        /* Compute candidate value: 30 * next_candidate + 13. */
        seq_t val = (seq_t)30 * (seq_t)next_candidate + 13;

        /* Test primality. */
        if (is_prime_big(val)) {
            /* Ensure enough room in the cache. */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();       /* allocation failed – terminate */
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the qualifying n. */
            cache[cache_size++] = (seq_t)next_candidate;
        }
        /* Move to the next integer. */
        next_candidate++;
    }

    return cache[n - 1];
}
