/*
 * A060978  |First digit - second digit + third digit - fourth digit ...| =
 *          10.
 * https://oeis.org/A060978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alt_sum_abs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function to compute A060978.
   a(n) = nth positive integer whose alternating sum of decimal
   digits (starting with + on the most significant digit)
   has absolute value 10, i.e. |d_k - d_{k-1} + d_{k-2} - ...| = 10. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alt_sum_abs.h"

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of cached terms */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_num = 1;     /* next integer to inspect */

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        /* check whether next_num satisfies the property */
        if (alt_sum_abs(next_num) == 10) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_num;
        }
        ++next_num;
    }

    return cache[n - 1];
}
