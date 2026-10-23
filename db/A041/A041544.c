/*
 * A041544  Numerators of continued fraction convergents to sqrt(290).
 * https://oeis.org/A041544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 34*a(n-1)+a(n-2) (n > 1), a(0)=17, a(1)=579
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return (seq_t)17;
    if (n == 1) return (seq_t)579;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache if necessary */
    if (!cache) {
        cache_size = 2;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* unlikely, but safe */
        cache[0] = (seq_t)17;
        cache[1] = (seq_t)579;
    }

    /* Extend cache to cover the required index */
    if (n >= cache_size) {
        idx_t new_size = n + 1;  /* allocate up to n inclusive */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* fallback in case of allocation failure */
        /* Compute new terms using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            new_cache[i] = (seq_t)34 * new_cache[i-1] + new_cache[i-2];
        }
        cache     = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
