/*
 * A203231  (n-1)-st elementary symmetric function of the first n terms of the
 *          periodic sequence (3,1,3,1,3,1,3,1,...).
 * https://oeis.org/A203231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-2) - 9*a(n-4) for n ≥ 5, seed values: a(1)=1,
 *            a(2)=4, a(3)=15, a(4)=24
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state: cache of previously computed terms */
    static seq_t *cache      = NULL;      /* array of results (index 0 unused) */
    static idx_t cache_cap   = 0;         /* current allocated size (≥ 5) */
    static idx_t cache_size  = 0;         /* highest index computed */

    if (n < 1)
        return (seq_t)0;                 /* undefined below the offset */

    /* Allocate or resize the cache on first call or when needed */
    if (cache == NULL)
    {
        idx_t init_cap = (n + 1 > 5) ? (n + 1) : 5;
        cache = (seq_t*)malloc(sizeof(seq_t) * init_cap);
        if (cache == NULL)
            return (seq_t)0;             /* allocation error */
        /* initialise known terms */
        cache[0] = (seq_t)0;            /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)24;
        cache_cap  = init_cap;
        cache_size = 4;
    }
    else if (n + 1 > cache_cap)
    {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL)
            return (seq_t)0;             /* realloc failed, unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    idx_t start = (cache_size + 1 > 5) ? (cache_size + 1) : 5;
    for (idx_t i = start; i <= n; ++i)
    {
        seq_t term1 = (seq_t)6 * cache[i - 2];
        seq_t term2 = (seq_t)9 * cache[i - 4];
        cache[i] = term1 - term2;
    }
    if (n > cache_size)
        cache_size = n;

    return cache[n];
}
