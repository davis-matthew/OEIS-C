/*
 * A052899  Expansion of g.f.: (1-2*x) / ((x-1)*(4*x^2+2*x-1)).
 * https://oeis.org/A052899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(n+2) = 4*a(n)+2*a(n+1)-1 for n ≥ 0 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;

    if (n >= cache_size) {
        /* enlarge cache to hold terms up to n */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!new_cache) abort();                /* allocation failed */
        cache = new_cache;
        cache_size = new_size;

        /* compute (or recompute) all terms up to n */
        for (size_t i = 0; i < (size_t)cache_size; ++i) {
            if (i == 0)
                cache[0] = (seq_t)1;
            else if (i == 1)
                cache[1] = (seq_t)1;
            else
                cache[i] = 4 * cache[i - 2] + 2 * cache[i - 1] - (seq_t)1;
        }
    }

    return cache[n];
}
