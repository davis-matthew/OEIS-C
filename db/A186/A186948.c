/*
 * A186948  a(n) = 3^n - 2*n.
 * https://oeis.org/A186948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2*n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* Direct cache of computed terms.  ::cache stores a() values
       for indices 0 .. cache_len-1. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of cached elements */

    /* If already cached, just return it. */
    if (n < cache_len)
        return cache[n];

    /* Need to extend the cache up to n. */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)   /* allocation failed – give up (unlikely for small n). */
        return 0;
    cache = tmp;

    /* Compute and cache the new terms.  Use the recurrence
       a(k) = 3*a(k-1) + 2*(2*k-3) for k >= 2. */
    for (idx_t k = cache_len; k <= n; ++k) {
        if (k == 0) {
            cache[k] = (seq_t)1;
        } else if (k == 1) {
            cache[k] = (seq_t)1;
        } else {
            seq_t term = 3 * cache[k - 1] + (seq_t)2 *
                ( (seq_t)( (idx_t)2 * k - 3 ) );
            cache[k] = term;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
