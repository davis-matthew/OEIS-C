/*
 * A090301  a(n) = 15*a(n-1) + a(n-2), starting with a(0) = 2 and a(1) = 15.
 * https://oeis.org/A090301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 15*a(n-1) + a(n-2), a(0) = 2, a(1) = 15
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <cstddef>
#endif

/* The function returns the n‑th term of the sequence A090301.
   It uses a static cache so that calls with arbitrary n are handled
   in O(n) time and in any order.  The cache is grown geometrically
   as needed.  The code works for both built‑in integer types and
   for arbitrary‑precision integer classes (C++ only).  */
seq_t a(idx_t n)
{
    /* static cache so the sequence can be computed in any call order */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* current allocated capacity   */
    static size_t last_computed = 1;  /* index of last computed value */

    /* initialise cache with first two terms if it does not exist yet */
    if (!cache) {
        cache_cap = 2;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!cache) return (seq_t)0;   /* allocation failure – return 0 fallback */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)15;
        last_computed = 1;
    }

    /* if the requested index is already cached, return it immediately */
    if ((idx_t)n <= (idx_t)last_computed)
        return cache[(size_t)n];

    /* ensure that the cache is large enough to store a[n] */
    if ((idx_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while ((idx_t)new_cap <= (idx_t)n + 1) new_cap <<= 1;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
#else
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;
#endif
        if (!new_cache) return (seq_t)0;  /* allocation failure */

        /* copy old values */
        for (size_t i = 0; i < cache_cap; ++i) {
            new_cache[i] = cache[i];
        }

        /* free the old array */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* compute all terms from last_computed+1 up to n */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)15 * cache[i - 1] + cache[i - 2];
    }
    last_computed = (size_t)n;

    return cache[(size_t)n];
}
