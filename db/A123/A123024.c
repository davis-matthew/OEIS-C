/*
 * A123024  a(n) = n!*b(n) where b(n) = (b(n-2) + b(n-3))/(n*(n-1)), b(0) =
 *          b(1) = b(2) = 1.
 * https://oeis.org/A123024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + (n-2) * a(n-3), with initials a(0) = 1, a(1) =
 *            1, a(2) = 2
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 26, u64 n <= 44, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <cstddef>   /* for size_t */
#endif

/* Helper to grow an allocated array of seq_t in a C++ safe way. */
static void *grow_array(seq_t *old, size_t old_cap, size_t new_cap)
{
#if defined(__cplusplus)
    /* Allocate new array with constructor calls. */
    seq_t *new_arr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_arr[i] = old[i];
    delete[] old;
    return new_arr;
#else
    /* In C, use realloc.  Objects of POD type are fine. */
    return realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* number of elements currently valid */

    /* Initialise with initial values if first call. */
    if (cache == NULL) {
        cache_cap = 3;
#if defined(__cplusplus)
        cache = new seq_t[cache_cap];
#else
        cache = malloc(cache_cap * sizeof(seq_t));
#endif
        if (cache == NULL) return (seq_t)0;  /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
    }

    /* Grow cache if needed to hold all terms up to n. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)grow_array(cache, cache_cap, new_cap);
        if (tmp == NULL) return (seq_t)0;  /* allocation failure */
        cache       = tmp;
        cache_cap   = new_cap;
    }

    /* Compute missing terms up to n. */
    size_t start = 3;
    /* In case cache was empty (unlikely after init). */
    if (cache_cap < 3) start = cache_cap;
    for (size_t i = start; i <= (size_t)n; ++i) {
        /* a(i) = a(i-2) + (i-2) * a(i-3) */
        seq_t term = cache[i - 2] + ((seq_t)(i - 2) * cache[i - 3]);
        cache[i] = term;
    }

    return cache[(size_t)n];
}
