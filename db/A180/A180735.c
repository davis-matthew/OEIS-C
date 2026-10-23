/*
 * A180735  Expansion of (1+x)*(1-x)/(1 - x + x^2 + x^3).
 * https://oeis.org/A180735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - a(n-2) - a(n-3) for n > 3, with a(0)=1, a(1)=1,
 *            a(2)=-1, a(3)=-3
 * Range:     i8 n <= 15, i16 n <= 34, i32 n <= 70, i64 n <= 142,
 *            i128 n <= 288, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;       /* array of computed values */
static size_t cache_size = 0;     /* current array length */

/* Compute a(n) using the recurrence and memoisation. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        assert(cache != NULL);
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = -1;
        cache[3] = -3;
    }

    /* Return already computed value if possible. */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Grow the cache to hold all indices up to n. */
    size_t new_size = (size_t)n + 1;
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        assert(tmp != NULL);
        cache = tmp;
        /* Compute newly added entries. */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            /* a(i) = a(i-1) - a(i-2) - a(i-3) */
            cache[i] = cache[i - 1] - cache[i - 2] - cache[i - 3];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
