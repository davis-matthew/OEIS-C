/*
 * A121965  a(n) = (n-1)*a(n-1)-a(n-2), a(0)=0, a(1)=1.
 * https://oeis.org/A121965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(n) = (n-1)*a(n-1) - a(n-2) for n>=2
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(idx_t n) returns the nth term of OEIS A121965 using a static cache. */

#include <stdlib.h>

/* External integer types supplied by build */
extern seq_t a(idx_t n);

/* Implements the sequence with memoisation. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation: NULL indicates uninitialised. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache for the first two terms. */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) {          /* allocation failure */
            return 0;
        }
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 2;
    }

    /* Return cached value if already computed. */
    if (n < cache_size) {
        return cache[n];
    }

    /* Expand cache to include index n. */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    if (tmp == NULL) {                 /* realloc failed */
        return 0;
    }
    cache = tmp;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size; i < new_size; ++i) {
        cache[i] = (seq_t)(i - 1) * cache[i - 1] - cache[i - 2];
    }
    cache_size = new_size;

    return cache[n];
}
