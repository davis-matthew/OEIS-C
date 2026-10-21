/*
 * A081408  a(n) = (n+1)*a(n-5), with a(0) = a(1) = a(2) = a(3) = a(4) = 1.
 * https://oeis.org/A081408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-5), a(0)=a(1)=a(2)=a(3)=a(4)=1
 * Range:     u8 n <= 14, u16 n <= 23, u32 n <= 39, u64 n <= 65, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation buffer */
static size_t cache_size = 0;    /* number of entries stored */

seq_t a(idx_t n)
{
    /* initialise the cache if it does not yet exist */
    if (cache == NULL) {
        /* allocate space for the first five terms */
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        for (idx_t i = 0; i < 5; ++i) cache[i] = (seq_t)1;
    }

    /* extend the cache to include n if needed */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        for (size_t i = cache_size; i < new_size; ++i) {
            idx_t idx = (idx_t)i;
            cache[i] = ((seq_t)(idx + 1) * cache[i - 5]);
        }
        cache_size = new_size;
    }

    return cache[n];
}
