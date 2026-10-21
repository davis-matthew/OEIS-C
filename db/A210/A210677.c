/*
 * A210677  a(n) = a(n-1) + a(n-2) + n + 1, a(0) = a(1) = 1.
 * https://oeis.org/A210677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + n + 1, a(0) = a(1) = 1 */
#include <stdlib.h>

/* Compute the A210677 sequence term a(n). */
seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of entries written */

    /* Initialize cache with first two values if necessary */
    if (cache == NULL)
    {
        /* Reserve space for at least 2 terms */
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* Grow cache up to index n if needed. */
    if ((idx_t)(n + 1) > cache_size)
    {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failure */
        cache = tmp;

        /* Compute missing terms iteratively. */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            cache[i] = cache[i-1] + cache[i-2] + (seq_t)i + (seq_t)1;
        }
        cache_size = new_size;
    }

    return cache[n];
}
