/*
 * A030186  a(n) = 3*a(n-1) + a(n-2) - a(n-3) for n >= 3, a(0)=1, a(1)=2,
 *          a(2)=7.
 * https://oeis.org/A030186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - a(n-3) with a(0)=1, a(1)=2, a(2)=7
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#  include <new>
#  include <cstring>
#else
#  include <string.h>
#endif

static seq_t *cache = (seq_t *)NULL;  /* array of computed terms */
static size_t cache_capacity = 0;     /* allocated size */
static size_t cache_size     = 0;     /* number of entries computed (>=1) */

static void grow_cache_to(idx_t n)
{
    size_t target = (size_t)n + 1;           /* we need indices 0..n inclusive */
    /* enlarge allocation if necessary */
    if (cache_capacity < target) {
        size_t new_cap = cache_capacity ? cache_capacity << 1 : 4;
        if (new_cap < target) new_cap = target;
#ifdef __cplusplus
        seq_t *newptr = (seq_t *)::operator new(new_cap * sizeof(seq_t));
#else
        seq_t *newptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
        /* copy existing elements to new block */
        if (cache) {
#ifdef __cplusplus
            for (size_t i = 0; i < cache_size; ++i)
                new (newptr + i) seq_t(cache[i]);   /* copy‑ctor or trivial */
            for (size_t i = 0; i < cache_size; ++i)
                cache[i].~seq_t();                   /* destructor */
            ::operator delete(cache);
#else
            memcpy(newptr, cache, cache_size * sizeof(seq_t));
            free(cache);
#endif
        }
        cache = newptr;
        cache_capacity = new_cap;
    }
    /* initialise base cases if they are not yet present */
    if (cache_size < 3) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache_size = 3;
    }
    /* compute missing entries up to n */
    if (cache_size < target) {
        for (size_t i = cache_size; i < target; ++i) {
            cache[i] = cache[i - 1] * (seq_t)3
                       + cache[i - 2]
                       - cache[i - 3];
        }
        cache_size = target;
    }
}

seq_t a(idx_t n)
{
    grow_cache_to(n);
    return cache[(size_t)n];
}
