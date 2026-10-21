/*
 * A056015  a(n) = 6*a(n-1) - 5*a(n-2) - 4*a(n-3) - 3*a(n-4) + 2*a(n-5) +
 *          a(n-6), with a(0)=...=a(4)=0, a(5)=1.
 * https://oeis.org/A056015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 5*a(n-2) - 4*a(n-3) - 3*a(n-4) + 2*a(n-5) +
 *            a(n-6) * Initial values: a(0)=a(1)=a(2)=a(3)=a(4)=0, a(5)=1
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* static cache for memoisation – shared across all calls */
static seq_t *cache       = NULL;          /* array of computed a(i) */
static idx_t cache_size   = 0;             /* allocated size of cache */
static idx_t last_computed = (idx_t)(-1);   /* highest index that has a value */

/*  Return the n‑th term of A056015.  The function may be called in
 *  any order; it lazily extends its cache.
 */
seq_t a(idx_t n)
{
    /* initialise cache on the first call */
    if (cache == NULL) {
        idx_t init_size = 6;               /* need indices 0…5 at start */
#ifdef __cplusplus
        cache = new seq_t[init_size]();    /* value‑initialised */
#else
        cache = (seq_t *)calloc((size_t)init_size, sizeof(seq_t));
#endif
        if (cache == NULL) abort();       /* allocation failure */
        cache[5] = (seq_t)1;              /* a(5) = 1   */
        cache_size = init_size;
        last_computed = 5;
    }

    /* grow the cache if it is too small for the requested n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_size]();
        for (idx_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        delete[] cache;
#else
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        for (idx_t i = cache_size; i < new_size; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
#endif
        cache      = tmp;
        cache_size = new_size;
    }

    /* compute any missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 4) {
            cache[i] = (seq_t)0;
        } else if (i == 5) {
            cache[i] = (seq_t)1;
        } else {
            cache[i] =  (6 * cache[i-1])
                       - (5 * cache[i-2])
                       - (4 * cache[i-3])
                       - (3 * cache[i-4])
                       + (2 * cache[i-5])
                       +   cache[i-6];
        }
    }

    last_computed = n;
    return cache[n];
}
