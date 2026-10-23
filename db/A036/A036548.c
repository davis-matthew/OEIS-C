/*
 * A036548  a(n) = T(7,n), array T given by A048471.
 * https://oeis.org/A036548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64*3^n - 63, which satisfies the recurrence * a(0) = 1, *
 *            a(n) = 3*a(n-1) + 126 (for n >= 1)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache with memoisation of already computed terms */
    static seq_t *cache = NULL;         /* pointer to the cached values */
    static size_t cache_size = 0;       /* number of terms currently stored (0 means none) */

    /* Ensure the cache is initialised with a(0) = 1 */
    if (cache_size == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) exit(1);            /* allocation failure */
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* If the requested index is already cached, return it */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Need to extend the cache up to index n */
    size_t target = (size_t)n;
    size_t new_size = target + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);                  /* allocation failure */
    cache = tmp;

    /* Compute missing terms using the recurrence a(i) = 3*a(i-1)+126 */
    for (size_t i = cache_size; i <= target; ++i) {
        cache[i] = cache[i - 1] * (seq_t)3 + (seq_t)126;
    }
    cache_size = new_size;

    return cache[target];
}
