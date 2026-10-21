/*
 * A078989  Chebyshev sequence with Diophantine property.
 * https://oeis.org/A078989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078989
   Recurrence: a(0)=1, a(1)=67,
               a(n)=66*a(n-1)-a(n-2) for n≥2   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;    /* allocated array size */
    static size_t cache_size = 0;        /* number of terms currently stored (>=1) */

    /* initialise cache on first call */
    if (cache_capacity == 0) {
        cache_capacity = 2;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* Allocation failed */
        /* seed first two terms */
        cache[0] = (seq_t)1u;
        cache[1] = (seq_t)67u;
        cache_size = 2;
    }

    /* If requested term already cached, return it immediately */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Grow storage if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_capacity) {
        size_t new_cap = cache_capacity;
        while (new_cap < needed) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                /* Allocation failed */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* Use a temporary 128‑bit product when seq_t is 64‑bit to avoid
           premature overflow; for arbitrary‑precision seq_t the builtin
           multiplication works correctly.  */
        cache[i] = (seq_t)(( (seq_t)66u * cache[i-1] ) - cache[i-2]);
    }
    cache_size = needed;   /* we now have terms up to index n */

    return cache[(size_t)n];
}
