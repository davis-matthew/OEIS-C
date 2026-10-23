/*
 * A112476  Diagonal sums of Riordan array (1/(1+x),x(1+x)/(1-x)).
 * https://oeis.org/A112476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-2) + 2*a(n-3) + a(n-4) Initial values: a(0)=1,
 *            a(1)=-1, a(2)=2, a(3)=0
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 38, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   array_resize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* size_t for C */
#include <stdlib.h>  /* malloc, realloc, free */
#include "../utils/array_resize.h"

/* Helper to resize an array of seq_t in a way that works for both
   C (trivial types) and C++ (arbitrary‑precision types).
   In C++, a new array is allocated, old elements are copied,
   and the old array is deleted. */
#ifdef __cplusplus

#else

#endif

seq_t a(idx_t n)
{
    /* Static cache for already computed terms */
    static seq_t *cache = NULL;      /* array of seq_t values */
    static idx_t cache_len = 0;      /* number of stored entries */

    /* Ensure the cache has at least the four base elements */
    if (cache_len < 4)
    {
        idx_t new_len = 4;
        seq_t *tmp = array_resize(cache, cache_len, new_len);
        cache = tmp;
        cache_len = new_len;

        /* Initialize the first four terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)0;
    }

    if (n < 4)
        return cache[n];

    /* Extend the cache if necessary */
    if (n >= cache_len)
    {
        idx_t new_len = n + 1;
        seq_t *tmp = array_resize(cache, cache_len, new_len);
        cache = tmp;

        /* Compute missing entries using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            cache[i] = cache[i - 2] * ((seq_t)2) +
                       cache[i - 3] * ((seq_t)2) +
                       cache[i - 4];
        }
        cache_len = new_len;
    }

    return cache[n];
}
