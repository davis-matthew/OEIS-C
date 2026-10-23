/*
 * A128919  Numbers which are both heptagonal and centered heptagonal.
 * https://oeis.org/A128919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 142*a(n-1) - a(n-2) + 7 (a(1)=1, a(2)=148)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* highest index stored in cache */

    if (n < 1) return 0; /* undefined for n < 1 */

    /* Initialize cache with first two terms */
    if (cache_size < 2) {
        cache_size = 2;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t)); /* indices 0..2 */
        if (!cache) return 0; /* allocation failed */
        cache[0] = (seq_t)0;        /* unused */
        cache[1] = (seq_t)1;        /* a(1) */
        cache[2] = (seq_t)148;     /* a(2) */
    }

    /* Extend cache if needed */
    if (n > cache_size) {
        size_t new_size = (size_t)n + 1; /* +1 for index 0 unused */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed */
        cache = new_cache;
        /* Compute missing values using recurrence */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = (seq_t)((seq_t)142 * cache[i - 1] - cache[i - 2] + (seq_t)7);
        }
        cache_size = n;
    }

    return cache[(size_t)n];
}
