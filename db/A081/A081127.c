/*
 * A081127  11th binomial transform of (0,1,0,0,0,0,0,...).
 * https://oeis.org/A081127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * 11^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=0, a(1)=1,
   a(n) = 22*a(n-1) - 121*a(n-2) for n >= 2
   (recurrence uses only small constants, so it
   can be computed iteratively with very low cost). */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Dynamic cache of previously computed terms.
       The cache is a simple array that grows when
       a larger index is requested.  This works in
       both C and C++.  The implementation uses only
       the operations allowed on seq_t ( +, -, *) and
       does not rely on any particular integer size.  */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;     /* number of terms stored (cache_size >= 2 after init) */
    static size_t cache_capacity = 0; /* allocated capacity */

    /* initialise the cache on first call */
    if (cache == NULL) {
        cache_capacity = 4;                       /* initial capacity, >= 2 */
        cache = (seq_t *)realloc(NULL, cache_capacity * sizeof(seq_t));
        cache[0] = (seq_t)0;                      /* a(0) */
        cache[1] = (seq_t)1;                      /* a(1) */
        cache_size = 2;
    }

    /* grow the cache until it can hold the requested index */
    while (cache_size <= n) {
        /* compute next value using the recurrence */
        seq_t prev   = cache[cache_size - 1];   /* a(n-1) */
        seq_t prev2  = cache[cache_size - 2];   /* a(n-2) */
        seq_t next   = (seq_t)22 * prev - (seq_t)121 * prev2;

        /* enlarge array if necessary */
        if (cache_capacity == cache_size) {
            /* double capacity, but never shrink */
            size_t new_cap = cache_capacity * 2;
            if (new_cap < cache_capacity + 1) new_cap = cache_capacity + 1; /* avoid overflow */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_capacity = new_cap;
        }
        cache[cache_size] = next;
        ++cache_size;
    }

    return cache[n];
}
