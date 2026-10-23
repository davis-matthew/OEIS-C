/*
 * A036554  Numbers whose binary representation ends in an odd number of
 *          zeros.
 * https://oeis.org/A036554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 2*A003159(n)  (numbers whose binary representation ends in an odd number of zeros) */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored sequence values */
    static size_t cache_cap = 0;  /* allocated capacity */
    static size_t cache_size = 0; /* number of terms cached */
    static idx_t next_candidate = 2; /* next integer to test for validity */

    /* Ensure the cache can store at least n elements */
    if ((idx_t)cache_cap < n) {
        /* Allocate a new buffer of the required size */
        seq_t *new_cache = alloc_seq_array((size_t)n);
        /* Copy existing values (if any) */
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        /* Free the old buffer (no need to destroy any values, they were copied) */
        if (cache != NULL)
            free_seq(cache);
        cache = new_cache;
        cache_cap = n;
    }

    /* Generate terms until we have at least n entries */
    while (cache_size < (size_t)n) {
        /* Find the next integer whose binary representation ends with an odd
           number of zeros (i.e., has odd 2-adic valuation). */
        while (1) {
            seq_t val = (seq_t)next_candidate;
            /* Count trailing zeros */
            idx_t tz = 0;
            while ((val & 1) == 0) {
                ++tz;
                val >>= 1;
            }
            if (tz & 1)            /* odd number of trailing zeros */
                break;
            ++next_candidate;      /* skip this value */
        }

        cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;          /* advance for the next round */
    }

    return cache[n - 1];
}
