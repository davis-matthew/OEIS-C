/*
 * A052638  E.g.f. x^2*(1+x-2x^2)/(1-2x).
 * https://oeis.org/A052638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)=0 for n<2; a(n)=n!*2^(n-2) for n>=2 (equivalently a(0)=0,
 *            a(1)=0, a(2)=2, a(3)=18, a(4)=96, and for n≥5: a(n)=2*n*a(n-1))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* shortcuts for the first five terms */
    static const seq_t first5[5] = { 0, 0, 2, 18, 96 };
    if (n < 5) return first5[n];

    /* lazy static cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialise the cache to hold the first five terms */
    if (cache_size == 0) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failed; fall back to 0 */
        for (idx_t i = 0; i < 5; ++i) cache[i] = first5[i];
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t old_size = cache_size;
        cache_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (new_cache == NULL) return 0; /* allocation failed; fall back to 0 */
        cache = new_cache;
        /* fill in previously uncomputed entries */
        for (idx_t i = old_size; i <= n; ++i) {
            /* recurrence: a(i) = 2*i*a(i-1) for i >= 5 */
            seq_t factor = (seq_t)2 * (seq_t)i;
            cache[i] = cache[i-1] * factor;
        }
    }

    return cache[n];
}
