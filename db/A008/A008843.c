/*
 * A008843  Squares of NSW numbers (A002315): x^2 such that x^2 - 2y^2 = -1
 *          for some y.
 * https://oeis.org/A008843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - a(n-2) + 16 (a(0)=1, a(1)=49)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* allocate an array of seq_t */
static seq_t *alloc_seq_arr(idx_t cap)
{
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t *)malloc((size_t)cap * sizeof(seq_t));
#endif
}

/* reallocate an array of seq_t */
static seq_t *realloc_seq_arr(seq_t *old, idx_t old_cap, idx_t new_cap)
{
    seq_t *new_arr = alloc_seq_arr(new_cap);
    if (!new_arr) return NULL;
    for (idx_t i = 0; i < (old_cap > new_cap ? new_cap : old_cap); ++i)
        new_arr[i] = old[i];
#if defined(__cplusplus)
    delete[] old;
#else
    free(old);
#endif
    return new_arr;
}

/* Compute the sequence */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* capacity of cache */
    static idx_t cache_len = 0;   /* number of elements stored */

    /* base values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)49;

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 2;
        cache = alloc_seq_arr(cache_cap);
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)49;
        cache_len = 2;
    }

    /* no need to recompute if already cached */
    if (cache_len > n)
        return cache[(size_t)n];

    /* grow cache if necessary */
    if ((idx_t)(n + 1) > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < (n + 1)) new_cap = n + 1;
        seq_t *tmp = realloc_seq_arr(cache, cache_cap, new_cap);
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)34 * cache[(size_t)(i - 1)] -
                   cache[(size_t)(i - 2)] + (seq_t)16;
    }
    cache_len = n + 1;
    return cache[(size_t)n];
}
