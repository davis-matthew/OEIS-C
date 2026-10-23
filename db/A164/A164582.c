/*
 * A164582  a(n) = 5*a(n - 1) - a(n - 2), with n>2, a(1)=2, a(2)=3.
 * https://oeis.org/A164582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n - 1) - a(n - 2), a(1) = 2, a(2) = 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* offset 1: undefined for n==0 */

    /* static state for memoisation */
    static seq_t *cache     = NULL;   /* array of computed values, index 0 unused */
    static idx_t cache_size = 0;      /* number of elements allocated (>= n+1) */
    static idx_t max_cached = 0;      /* highest index that has a valid value */

    /* first call: allocate at least enough space for indices 1 and 2 */
    if (cache_size == 0) {
        idx_t init_size = n + 1 > 3 ? n + 1 : 3;   /* ensure indices 1,2 exist */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;       /* allocation failed */
        cache_size = init_size;
        cache[0] = 0;
        cache[1] = (seq_t)2;        /* a(1) */
        cache[2] = (seq_t)3;        /* a(2) */
        max_cached = 2;
    }

    /* grow cache if required */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* allocation failed */
        cache     = tmp;
        cache_size = new_size;
    }

    /* return cached value if already known */
    if (n <= max_cached)
        return cache[n];

    /* compute missing terms */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] = ((seq_t)5 * cache[i - 1]) - cache[i - 2];
    }
    max_cached = n;
    return cache[n];
}
