/*
 * A064098  a(n+1) = (a(n)^2 + a(n-1)^2)/a(n-2), with a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A064098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064098: a(n+1) = (a(n)^2 + a(n-1)^2) / a(n-2)
   with a(1) = a(2) = a(3) = 1.
   The implementation uses a simple static cache so that calls
   may arrive in arbitrary order yet every term is computed only once. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* caching: cache[0] is unused; cache[1]..cache[cache_len] hold computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* maximum index that the allocated area covers */
    static idx_t cache_len = 0;   /* highest index already computed */

    if (n == 0)                     /* sequence starts at index 1 */
        return (seq_t)0;

    /* ensure the buffer is large enough to hold a[n] */
    if (n > cache_cap)
    {
        idx_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        while (new_cap < n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (!tmp)
            exit(1);              /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first three terms (they are 1) */
    if (cache_len < 3)
    {
        if (cache_cap < 3)
        {
            /* guard against the unlikely case that cache_cap < 3 */
            idx_t new_cap = 3;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
            if (!tmp)
                exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache_len = 3;
    }

    /* compute terms up to index n */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t t1 = cache[i - 1] * cache[i - 1];
        seq_t t2 = cache[i - 2] * cache[i - 2];
        cache[i] = (t1 + t2) / cache[i - 3];
    }

    if (n > cache_len)
        cache_len = n;

    return cache[n];
}
