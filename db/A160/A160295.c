/*
 * A160295  Numerator of Hermite(n, 17/30).
 * https://oeis.org/A160295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 17*a(n-1) - 450*(n-1)*a(n-2), a(0)=1, a(1)=17
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_capacity = 0;   /* allocated size of cache */
static idx_t last_computed = 1;    /* largest index that has been computed (>=1) */
static int initialized = 0;       /* flag whether the cache has been initialised */

/* Return the n-th term of the sequence A160295 */
seq_t a(idx_t n)
{
    /* Initialise cache for the first call */
    if (!initialized)
    {
        cache_capacity = (n + 1 > 2 ? n + 1 : 2);   /* at least two slots */
        cache = (seq_t*)realloc(cache, cache_capacity * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)17;
        last_computed = 1;
        initialized = 1;
    }

    /* Grow the cache if necessary */
    if (cache_capacity <= n)
    {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    if (n > last_computed)
    {
        for (idx_t k = last_computed + 1; k <= n; ++k)
        {
            seq_t factor = (seq_t)(k - 1) * (seq_t)450;   /* 450*(k-1) */
            cache[k] = (seq_t)(17 * cache[k - 1] - factor * cache[k - 2]);
        }
        last_computed = n;
    }

    return cache[n];
}
