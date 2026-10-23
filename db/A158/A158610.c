/*
 * A158610  Expansion of (1+15*x)/(1-x-256*x^2).
 * https://oeis.org/A158610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 256*a(n-2), a(0)=1, a(1)=16
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t size = 0;     /* number of initialised elements */
    static size_t cap  = 0;     /* allocated capacity */

    size_t idx = (size_t)n;

    /* grow cache capacity if necessary */
    if (idx >= cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= idx) new_cap <<= 1;    /* double until large enough */

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;        /* allocation failure */

        if (cache) {
            for (size_t i = 0; i < size; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute any missing terms up to idx */
    for (size_t i = size; i <= idx; ++i) {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = (seq_t)16;
        else
            cache[i] = cache[i-1] + (seq_t)256 * cache[i-2];
    }
    size = idx + 1;

    return cache[idx];
}
