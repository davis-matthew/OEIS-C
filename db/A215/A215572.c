/*
 * A215572  a(n) = 3*a(n-1) + 46*a(n-2) + a(n-3) with a(0)=2, a(1)=5,
 *          a(2)=106.
 * https://oeis.org/A215572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 46*a(n-2) + a(n-3) with a(0)=2, a(1)=5,
 *            a(2)=106
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Includes needed for C and C++ compilation */
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Static cache for computed terms */
static seq_t *cache = NULL;          /* pointer to array */
static size_t cache_size = 0;        /* number of valid terms stored */
static size_t cache_capacity = 0;    /* allocated capacity */

/* Grow cache so that it can hold at least 'required' terms */
static void grow_cache(size_t required)
{
    if (required <= cache_capacity)
        return;                    /* nothing to do */

    size_t new_capacity = required;
#if defined(__cplusplus)
    /* C++: allocate using operator new */
    seq_t *new_cache = static_cast<seq_t*>(::operator new(new_capacity * sizeof(seq_t)));
#else
    /* C: allocate with malloc */
    seq_t *new_cache = (seq_t*)malloc(new_capacity * sizeof(seq_t));
#endif

    /* copy already computed terms */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

#ifdef __cplusplus
    /* default‑construct new elements (needed for class types) */
    for (size_t i = cache_size; i < new_capacity; ++i)
        new (&new_cache[i]) seq_t();
#endif

    /* free old storage */
#if defined(__cplusplus)
    if (cache != NULL)
        ::operator delete(cache);
#else
    if (cache != NULL)
        free(cache);
#endif

    cache = new_cache;
    cache_capacity = new_capacity;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    /* Handle the three base cases directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)106;

    size_t idx = (size_t)n;          /* index in array */
    grow_cache(idx + 1);             /* ensure space */

    if (cache_size <= idx) {         /* need to compute new terms */
        /* initialise the first three terms if cache empty */
        if (cache_size == 0) {
            cache[0] = (seq_t)2;
            cache[1] = (seq_t)5;
            cache[2] = (seq_t)106;
            cache_size = 3;
        }

        /* compute remaining terms up to idx */
        for (size_t i = cache_size; i <= idx; ++i) {
            cache[i] = (seq_t)3 * cache[i-1] +
                       (seq_t)46 * cache[i-2] +
                       cache[i-3];
        }
        cache_size = idx + 1;
    }

    return cache[idx];
}
