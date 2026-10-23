/*
 * A144787  Recurrence sequence a(n+1)=a(n)^3-a(n)+1 and a(1)=2.
 * https://oeis.org/A144787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 2; a(n+1) = a(n)^3 - a(n) + 1 */
#include <stdlib.h>

/* Compute the OEIS sequence A144787.
   a(n) is defined by a(1)=2 and a(n+1)=a(n)^3-a(n)+1 (n≥1). */
seq_t a(idx_t n)
{
    /* static cache: cache[i] stores a(i) for 1≤i≤cache_size. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest index currently cached */

    /* If n is below the offset, return 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Grow the cache if necessary.  We allocate up to n (index 0 unused). */
    if ((idx_t)((size_t)n + 1) > cache_size) {
        size_t new_cap = (size_t)n + 1;            /* size in elements, idx 0 unused */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
#if defined(__cplusplus)
            std::terminate();
#else
            abort();
#endif
        }
        cache = tmp;

        /* First time we allocate: initialise a(1). */
        if (cache_size == 0) {
            cache[1] = (seq_t)2;
            cache_size = 1;
        }
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = prev * prev * prev - prev + (seq_t)1;
    }
    /* Update the highest cached index. */
    cache_size = n;

    return cache[n];
}
