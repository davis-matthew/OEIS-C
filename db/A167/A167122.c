/*
 * A167122  a(n) = 20*a(n-1) - 64*a(n-2) + 3 for n > 2; a(0) = 1, a(1) = 22,
 *          a(2) = 378.
 * https://oeis.org/A167122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 64*a(n-2) + 3 for n > 2; with initial values
 *            a(0)=1, a(1)=22, a(2)=378
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include "../utils/seq_alloc_array.h"   /* assumed available for seq_alloc_array */
#include "../utils/seq_free_array.h"    /* assumed available for seq_free_array */

seq_t a(idx_t n)
{
    /* cache the computed terms for repeated requests */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* capacity of the cache array       */
    static idx_t cached_max = (idx_t)-1; /* largest n whose term is in cache */

    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;
    if (n == 2) return (seq_t)378;

    /* initialise cache if needed */
    if (cache == NULL) {
        cache_len = 3;                 /* store 0,1,2 */
        cache     = seq_alloc_array(cache_len);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)378;
        cached_max = 2;
    }

    /* enlarge cache if the requested n is not yet computed */
    if ((idx_t)n > cached_max) {
        size_t new_len = (size_t)n + 1;
        /* allocate a bigger array, copy old values, and free old array */
        seq_t *new_cache = seq_alloc_array(new_len);
        for (size_t i = 0; i <= (size_t)cached_max; ++i)
            new_cache[i] = cache[i];
        seq_free_array(cache);
        cache = new_cache;
        cache_len = new_len;

        /* compute the missing values using the recurrence */
        for (idx_t i = cached_max + 1; i <= (idx_t)n; ++i) {
            cache[i] = (seq_t)20 * cache[i-1]
                     - (seq_t)64 * cache[i-2]
                     + (seq_t)3;
        }
        cached_max = (idx_t)n;
    }

    return cache[n];
}
