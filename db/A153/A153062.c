/*
 * A153062  a(0)=1, a(n)=a(n-1)^2-n^2.
 * https://oeis.org/A153062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 6, i64 n <= 7, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1)^2 - n^2  for n ≥ 1,  a(0) = 1
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of computed terms          */
    static size_t cache_size = 0;        /* number of valid entries          */
    static size_t cache_capacity = 0;    /* allocated array size             */

    /* initialise cache with a(0) */
    if (cache_size == 0) {
        size_t init_cap = 1;
#ifdef __cplusplus
        cache = new seq_t[init_cap];
#else
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
#endif
        if (!cache) return (seq_t)0;     /* allocation failure – return 0   */
        cache[0] = (seq_t)1;             /* a(0) = 1 */
        cache_size = 1;
        cache_capacity = init_cap;
    }

    /* compute terms up to index n if necessary */
    size_t idx = (size_t)n;
    if (idx >= cache_size) {
        size_t need_cap = idx + 1;
        if (need_cap > cache_capacity) {
#ifdef __cplusplus
            /* allocate a larger array and copy old values */
            seq_t *new_cache = new seq_t[need_cap];
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            delete[] cache;
            cache = new_cache;
            cache_capacity = need_cap;
#else
            seq_t *new_cache = (seq_t *)realloc(cache, need_cap * sizeof(seq_t));
            if (!new_cache) return (seq_t)0;   /* allocation failure – return 0   */
            cache = new_cache;
            cache_capacity = need_cap;
#endif
        }
        /* fill missing terms */
        for (size_t i = cache_size; i <= idx; ++i) {
            seq_t prev = cache[i - 1];
            seq_t sq   = prev * prev;                          /* a(i-1)^2   */
            seq_t ii   = (seq_t)i;
            cache[i]   = sq - ii * ii;                         /* a(i) = a(i-1)^2 - i^2 */
        }
        cache_size = need_cap;
    }

    return cache[idx];
}
