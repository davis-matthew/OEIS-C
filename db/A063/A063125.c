/*
 * A063125  Dimension of the space of weight 2n cusp forms for Gamma_0( 57 ).
 * https://oeis.org/A063125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) – a(n-4) for n > 5, with a(1)=5, a(2)=18,
 *            a(3)=32, a(4)=44, a(5)=58
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cached_up_to = 0;

    /* Handle n < 1 gracefully (outside offset) */
    if (n < 1)
        return (seq_t)0;

    /* Extend cache if necessary */
    if (n > cached_up_to) {
        /* Allocate/resize the cache up to index n */
        size_t new_size = (size_t)n + 1;               /* include index n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;                          /* allocation failed */
        cache = new_cache;

        /* Compute missing terms */
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
            if (i == 1)            cache[i] = (seq_t)5;
            else if (i == 2)       cache[i] = (seq_t)18;
            else if (i == 3)       cache[i] = (seq_t)32;
            else if (i == 4)       cache[i] = (seq_t)44;
            else if (i == 5)       cache[i] = (seq_t)58;
            else
                cache[i] = cache[i-1] + cache[i-3] - cache[i-4];
        }
        cached_up_to = n;
    }

    return cache[n];
}
