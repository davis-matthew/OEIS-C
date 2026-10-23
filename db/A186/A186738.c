/*
 * A186738  a(0)=0, a(1)=0; for n>1, a(n) = a(n-1) + (n-1)*a(n-2) + 1.
 * https://oeis.org/A186738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (n-1)*a(n-2) + 1; a(0)=0, a(1)=0
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 32, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;         /* dynamic cache for computed terms */
static size_t cache_cap = 0;        /* allocated capacity (number of elements) */
static idx_t  cache_len = 0;        /* number of terms currently computed */

/* Computes the OEIS sequence A186738 */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        size_t init_cap = 2;        /* store a(0) and a(1) */
        seq_t *ptr;
#ifdef __cplusplus
        ptr = new seq_t[init_cap];
#else
        ptr = (seq_t *)malloc(init_cap * sizeof(seq_t));
#endif
        ptr[0] = 0;
        ptr[1] = 0;
        cache      = ptr;
        cache_cap  = init_cap;
        cache_len  = 2;
    }

    /* Return already computed value */
    if (n < cache_len)
        return cache[n];

    /* Ensure sufficient capacity */
    idx_t needed = n + 1;   /* number of elements we need */
    if ((size_t)needed > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((size_t)needed > new_cap)
            new_cap <<= 1;

        seq_t *new_ptr;
#ifdef __cplusplus
        new_ptr = new seq_t[new_cap];
#else
        new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
        /* copy existing values */
        for (idx_t i = 0; i < cache_len; ++i)
            new_ptr[i] = cache[i];

#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache      = new_ptr;
        cache_cap  = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term = cache[i-1] + (seq_t)(i-1) * cache[i-2] + 1;
        cache[i] = term;
    }
    cache_len = n + 1;

    return cache[n];
}
