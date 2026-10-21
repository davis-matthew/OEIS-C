/*
 * A052639  E.g.f. (1-2x)/(1-2x-x^3).
 * https://oeis.org/A052639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=0, a(n+3)=
 *            ((11*n+6+n^3+6*n^2)*a(n)+(2*n+6)*a(n+2))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure cache is large enough */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;           /* out of memory guard */
        if (cache) {
            for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
            free(cache);
        }
        cache     = new_cache;
        cache_size = new_size;
    }

    /* Initialise first three values if they are not yet set */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache_size = 3;
    } else {
        /* If the cache was already initialised elsewhere, ensure the base
           values are set for safety. */
        if (n >= 0 && cache[0] != (seq_t)1) cache[0] = (seq_t)1;
        if (n >= 1 && cache[1] != (seq_t)0) cache[1] = (seq_t)0;
        if (n >= 2 && cache[2] != (seq_t)0) cache[2] = (seq_t)0;
    }

    /* Compute missing entries using the recurrence */
    for (idx_t i = 0; i + 3 <= n; ++i) {
        if (cache[i + 3]) continue; /* already computed */
        seq_t i_seq = (seq_t)i;
        seq_t i2    = i_seq * i_seq;
        seq_t i3    = i2 * i_seq;
        seq_t factor1 = 11 * i_seq + (seq_t)6 + i3 + 6 * i2;
        seq_t factor2 = 2 * i_seq + (seq_t)6;
        cache[i + 3] = factor1 * cache[i] + factor2 * cache[i + 2];
    }

    return cache[n];
}
