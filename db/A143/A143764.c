/*
 * A143764  a(n+1) = (a(n)+n)^2, a(1) = 1.
 * https://oeis.org/A143764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n+1) = (a(n)+n)^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Global cache for previously computed terms.
   cache[i] holds a(i+1)  (0‑based indexing).  */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity                */
static size_t cache_len = 0;   /* number of terms actually stored   */

/* Grow the cache so that it can hold at least 'needed' terms. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap < needed)
        new_cap = needed;

#ifndef __cplusplus
    /* C: realloc works for scalar seq_t types */
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)  /* allocation failure: abort */
        abort();
    cache = tmp;
#else
    /* C++: use operator new[] for objects (e.g. arbitrary‑precision). */
    seq_t *tmp = new seq_t[new_cap];
    /* Copy existing terms if any. */
    for (size_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];
    /* Deallocate old storage. */
    delete[] cache;
    cache = tmp;
#endif

    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* out of bounds – should never be requested */

    /* Initialise cache with the first term if necessary. */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = (seq_t)1;     /* a(1) = 1 */
        cache_len = 1;
    }

    /* Compute any missing terms up to n. */
    if ((size_t)n > cache_len) {
        grow_cache((size_t)n);
        /* loop indices are 0‑based: cache[i] == a(i+1) */
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            seq_t prev = cache[i - 1];          /* a(i)   = a(i)          */
            seq_t tmp  = prev + (seq_t)i;       /* a(i)+i                 */
            cache[i] = tmp * tmp;               /* a(i+1) = (a(i)+i)^2    */
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
