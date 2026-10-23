/*
 * A163310  a(n) = 20*a(n-1) - 95*a(n-2) for n > 1; a(0) = 1, a(1) = 11.
 * https://oeis.org/A163310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 11; a(n) = 20*a(n-1) - 95*a(n-2) for n > 1 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[i] = a(i) */
static size_t cache_size = 0;   /* allocated length of cache   */
static idx_t  cached_to  = (idx_t)-1; /* highest index initialized in cache */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    /* Initialize cache on first call */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cached_to = 1;
    }

    /* Return cached value if already computed */
    if (n <= cached_to) {
        return cache[(size_t)n];
    }

    /* Expand cache if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_size) {
        size_t new_size = cache_size == 0 ? 2 : cache_size;
        while (new_size < needed) {
            new_size *= 2;
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failure: abort */
            abort();
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute new terms up to n */
    for (size_t i = (size_t)(cached_to + 1); i <= (size_t)n; ++i) {
        cache[i] = (seq_t)20 * cache[i - 1] - (seq_t)95 * cache[i - 2];
    }
    cached_to = n;

    return cache[(size_t)n];
}
