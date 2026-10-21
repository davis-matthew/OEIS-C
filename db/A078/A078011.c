/*
 * A078011  Expansion of (1-x)/(1-x-x^2+2*x^3).
 * https://oeis.org/A078011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - 2*a(n-3), with a(0)=1, a(1)=0, a(2)=1
 * Range:     i8 n <= 21, i16 n <= 44, i32 n <= 90, i64 n <= 177,
 *            i128 n <= 351, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Cache to store computed terms */
    static seq_t *cache      = NULL;   /* pointer to array of terms */
    static size_t cache_size = 0;      /* number of allocated slots (≥ 1) */
    static idx_t  computed   = 0;      /* highest index computed so far */

    /* Initialise cache on first call */
    if (cache == NULL) {
        /* Need at least indices 0..2 */
        size_t sz = 3;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocate failure, return 0 */

        /* Set base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;

        cache_size = sz;
        computed   = 2;          /* we have computed up to index 2 */
    }

    /* Expand cache if a larger index is requested */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;  /* realloc failure, return 0 */
        cache      = new_cache;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    if (n > computed) {
        /* We already have terms 0..computed. Compute from computed+1 to n. */
        /* computed is always ≥ 2 after the first call. */
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* Recurrence: a(i) = a(i-1) + a(i-2) - 2*a(i-3) */
            cache[i] = cache[i-1] + cache[i-2] - (seq_t)2 * cache[i-3];
        }
        computed = n;
    }

    return cache[n];
}
