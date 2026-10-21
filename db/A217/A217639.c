/*
 * A217639  Number of nX2 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or antidiagonal neighbors
 *          in a random 0..2 nX2 array.
 * https://oeis.org/A217639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A217639: Number of n×2 arrays of the minimum value of corresponding
   elements and their horizontal, vertical or antidiagonal neighbors in a
   random 0..2 n×2 array.
   Recurrence: a(n) = 2*a(n-1)+3*a(n-2)+3*a(n-3)+2*a(n-4)-a(n-6)
   with a(1)=3, a(2)=9, a(3)=27, a(4)=91, a(5)=293, a(6)=959.
   The function memoises results in a dynamically resized static array,
   compatible with both C11 and C++17. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache and bookkeeping. */
    static seq_t *cache      = NULL;   /* array of computed terms, index 0 unused  */
    static idx_t cache_size  = 0;      /* total size allocated (including unused 0) */
    static idx_t max_computed = 0;     /* highest n that has been computed */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        /* Start with enough room for indices 0..6 (inclusive). */
        cache_size = 7;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();                /* allocation failure */
#ifdef __cplusplus
        /* In C++ construct the objects explicitly.  In C the objects are
           trivial (integer type) so this block is skipped. */
        for (idx_t i = 0; i < cache_size; ++i)
            new (cache + i) seq_t();
#endif
        /* Seed the known base values. */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)27;
        cache[4] = (seq_t)91;
        cache[5] = (seq_t)293;
        cache[6] = (seq_t)959;
        max_computed = 6;
    }

    /* Undefined for n = 0 in the OEIS offset 1 convention. */
    if (n == 0) return (seq_t)0;

    /* Grow cache if n exceeds current allocation. */
    while (n >= cache_size) {
        idx_t new_size = cache_size * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
#ifdef __cplusplus
        /* Construct any newly allocated elements. */
        for (idx_t i = cache_size; i < new_size; ++i)
            new (cache + i) seq_t();
#endif
        cache_size = new_size;
    }

    /* If value already known, return it. */
    if (n <= max_computed)
        return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 6) {
            /* Base values already set during initialisation; skip. */
            val = cache[i];
        } else {
            val =    (seq_t)2 * cache[i-1]
                   + (seq_t)3 * cache[i-2]
                   + (seq_t)3 * cache[i-3]
                   + (seq_t)2 * cache[i-4]
                   -           cache[i-6];
        }
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
