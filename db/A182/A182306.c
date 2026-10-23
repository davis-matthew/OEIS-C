/*
 * A182306  a(n+1) = a(n) + floor(a(n)/5) with a(0)=5.
 * https://oeis.org/A182306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + floor(a(n-1)/5), a(0) = 5
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 115, u64 n <= 236,
 *            u128 n <= 480, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values.
     * cache_end points to the largest index that has been computed.
     * cache_cap is the allocated capacity of the array.
     */
    static seq_t *cache = NULL;
    static idx_t cache_end = 0;     /* last index computed; 0 means none yet */
    static size_t cache_cap = 0;    /* allocated size */

    /* Ensure the cache has room for at least (n+1) entries. */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap <= n) new_cap <<= 1;  /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cache && !tmp) /* realloc failed, keep old buffer */
            return (seq_t)0;            /* should not happen in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the cache is empty, initialise a(0). */
    if (cache_end == 0 && n >= 0) {
        cache[0] = (seq_t)5;
        cache_end = 0;
    }

    /* Compute values up to n if needed. */
    while (cache_end < n) {
        seq_t prev = cache[cache_end];
        cache[cache_end + 1] = prev + prev / (seq_t)5;
        cache_end++;
    }

    return cache[n];
}
