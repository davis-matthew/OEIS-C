/*
 * A184679  Number of (n+1) X 3 binary arrays with every 2 X 2 subblock
 *          singular.
 * https://oeis.org/A184679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 12*a(n-3) Initial values: a(1)=28, a(2)=128,
 *            a(3)=544
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

static seq_t   *cache      = NULL;   /* array of precomputed terms, index 0 unused */
static idx_t   cache_cap   = 0;      /* maximum index that can be stored */
static idx_t   cache_sz    = 0;      /* highest index that has been computed */

/* Ensure that a(1)..a(n) are computed and stored in cache. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_sz)                      /* already computed */
        return;

    /* Allocate or enlarge the cache if necessary. */
    if (cache == NULL) {
        cache_cap = (n > 3 ? n : 4);
        cache = (seq_t*)malloc((cache_cap + 1) * sizeof(seq_t));
        if (cache == NULL)                  /* allocation fail – abort */
            return;
    } else if (n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL)                    /* realloc fail – abort */
            return;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t i = cache_sz + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = 28;
        else if (i == 2)
            cache[i] = 128;
        else if (i == 3)
            cache[i] = 544;
        else
            cache[i] = 5 * cache[i-1] - 12 * cache[i-3];
    }
    cache_sz = n;
}

seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for n<1, return 0 */
        return 0;

    ensure_cache(n);
    return cache[n];
}
