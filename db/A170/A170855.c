/*
 * A170855  a(0) = 0, a(1) = 1; a(2^i + j) = a(j) + 3a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170855
   a(0) = 0, a(1) = 1;
   For n >= 2:
       i = floor(log2(n))
       base = 2^i
       j = n - base
       a(n) = a(j) + 3 * a(j+1)     (0 <= j < base)
   The function caches all computed values to support arbitrary order of calls.
*/

#include <stdlib.h>
#include <stdint.h>

static idx_t highest_power_of_two(idx_t n) {
    /* Return largest power of two <= n (n >= 1) */
    idx_t p = 1;
    while (p <= n >> 1)
        p <<= 1;
    return p;
}

seq_t a(idx_t n) {
    static seq_t *cache   = NULL;
    static size_t cache_len = 0; /* number of computed entries */
    static size_t cache_cap = 0; /* storage capacity */

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    }

    /* If the value is already known, return it */
    if (n < cache_len)
        return cache[n];

    /* Ensure sufficient capacity for all indices up to n */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* Compute any missing values up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        idx_t base = highest_power_of_two(k);
        idx_t j    = k - base;
        cache[k] = cache[j] + (seq_t)3 * cache[j + 1];
    }
    cache_len = n + 1;

    return cache[n];
}
