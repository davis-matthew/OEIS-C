/*
 * A152927  Number of sets (in the Hausdorff metric geometry) at each location
 *          between two sets defining a polygonal configuration consisting of
 *          k 4-gonal polygonal components chained with string components of
 *          length 1 as k varies.
 * https://oeis.org/A152927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) + a(n-2) (a(0)=1, a(1)=7)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* for size_t */
#include <stdlib.h>    /* for malloc/free, NULL */

/* Compatibility macros for C and C++ */
#if defined(__cplusplus)
#define ALLOC_ARRAY(n, type) new type[(n)]
#define FREE_ARRAY(ptr) delete [] (ptr)
#else
#define ALLOC_ARRAY(n, type) (type*)malloc((size_t)(n)*sizeof(type))
#define FREE_ARRAY(ptr) free(ptr)
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed = 0;

    /* Expand cache if necessary */
    if (n >= cache_size) {
        /* Grow to the next power of two >= n+1 */
        idx_t new_size = cache_size > 0 ? cache_size << 1 : 16;
        while (new_size <= n) new_size <<= 1;

        seq_t *tmp = ALLOC_ARRAY(new_size, seq_t);
        if (!tmp) return (seq_t)0;           /* allocation failed */

        /* Copy old values if any */
        for (idx_t i = 0; i < cache_size; ++i) {
            tmp[i] = cache[i];
        }

        FREE_ARRAY(cache);
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise base terms if not yet done */
    if (computed == 0 && cache_size > 0) {
        cache[0] = (seq_t)1;
        if (cache_size > 1) cache[1] = (seq_t)7;
        computed = 2;
    }

    /* Compute a(n) for all missing indices up to n */
    if (n >= computed) {
        for (idx_t i = computed; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)7;
            } else {
                /* a(i) = 16*a(i-1) + a(i-2) */
                cache[i] = (seq_t)16 * cache[i - 1] + cache[i - 2];
            }
        }
        computed = n + 1;
    }

    return cache[n];
}
