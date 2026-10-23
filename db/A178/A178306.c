/*
 * A178306  a(0)=1, a(1)=a(2)=1 and a(3k) = a(k), a(3k+1) = a(k)+a(k+1),
 *          a(3k+2) = 2a(k) for k >= 1.
 * https://oeis.org/A178306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = a(2) = 1 and a(3k) = a(k), a(3k+1) =
 *            a(k)+a(k+1), a(3k+2) = 2*a(k) for k >= 1
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* static cache */
static seq_t *cache = NULL;      /* array of computed values          */
static idx_t cache_size = 0;     /* allocated size of the array       */
static idx_t computed_size = 0;  /* highest index that has been filled */

/* Ensure the cache contains all values up to n. */
static void ensure_cache(idx_t n)
{
    if (n < computed_size)
        return;                 /* nothing to do */

    /* Grow the array as necessary. */
    if ((idx_t)(n + 1) > cache_size) {
#ifdef __cplusplus
        /* In C++ we must use operator new[] and delete[] to construct
           arbitrary-precision objects.  Existing contents are copied to
           the new buffer. */
        seq_t *new_cache = new seq_t[n + 1];
        if (cache) {
            for (idx_t i = 0; i < computed_size; ++i)
                new_cache[i] = cache[i];
            delete[] cache;
        }
        cache = new_cache;
        cache_size = n + 1;
#else
        /* In plain C we can reuse malloc/realloc because the seq_t type
           is assumed to be a POD type in that configuration. */
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!cache)
            abort();           /* memory allocation failed */
        cache_size = n + 1;
#endif
    }

    /* Compute values from the first missing index up to n. */
    for (idx_t i = computed_size; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 0;                 /* a(0) = 1 is defined as 1; but a(0)=0 in the list? */
        } else if (i == 1 || i == 2) {
            val = 1;                 /* a(1) = a(2) = 1 */
        } else {
            idx_t k = i / 3;
            unsigned rem = (unsigned)(i % 3);
            if (rem == 0) {
                val = cache[k];
            } else if (rem == 1) {
                val = cache[k] + cache[k + 1];
            } else {                /* rem == 2 */
                val = cache[k] << 1;   /* multiply by 2 */
            }
        }
        cache[i] = val;
    }
    computed_size = n + 1;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
