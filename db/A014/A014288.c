/*
 * A014288  a(n) = floor(Sum_{k=0..n} k!/2), or floor( A003422(n+1)/2 ).
 * https://oeis.org/A014288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sum_{k=0..n} k! / 2 ) Recurrence: a(0)=0, a(1)=1,
 *            a(2)=2, a(n) = (n+1)*a(n-1) - n*a(n-2) (n>=3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, free, realloc */

/* The types seq_t and idx_t are supplied by the build environment.
   idx_t is an unsigned integer type (typically uint64_t). */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached sequence values */
    static idx_t   cap   = 0;     /* current allocation size */

    /* initialise cache with the first three known values */
    if (cache == NULL) {
        cap = 3;                     /* indices 0,1,2 */
#ifdef __cplusplus
        cache = new seq_t[cap];
#else
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
#endif
        if (!cache) return 0;         /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
    }

    /* extend cache if the requested index is beyond current size */
    if (n + 1 > cap) {
        idx_t old_cap = cap;
        idx_t new_cap = n + 1;
        seq_t *new_cache;

#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
#else
        new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
        if (!new_cache) return 0;      /* allocation failure */

        /* copy existing entries to the new buffer */
        for (idx_t i = 0; i < old_cap; ++i)
            new_cache[i] = cache[i];

        /* free old buffer */
#ifdef __cplusplus
        delete [] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cap   = new_cap;

        /* compute new entries using the recurrence */
        for (idx_t i = old_cap; i <= n; ++i) {
            seq_t term1 = (seq_t)(i + 1) * cache[i - 1];
            seq_t term2 = (seq_t)i * cache[i - 2];
            cache[i]   = term1 - term2;
        }
    }

    return cache[n];
}
