/*
 * A123393  Values X satisfying the equation 7(X-Y)^4-2XY=0, where X>=Y.
 * https://oeis.org/A123393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 928*a(n-1) - 26942*a(n-2) + 928*a(n-3) - a(n-4); Initial
 *            values: a(0)=0, a(1)=32, a(2)=27000, a(3)=24193888
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <new>
#endif

/* allocates an array of sz seq_t objects (C: malloc, C++: new) */
static seq_t *alloc_cache(size_t sz)
{
#ifdef __cplusplus
    return new seq_t[sz];
#else
    return (seq_t *)malloc(sz * sizeof(seq_t));
#endif
}

/* resizes an existing cache array from oldsz to newsz
   (C: realloc, C++: new/delete) */
static seq_t *resize_cache(seq_t *old, size_t oldsz, size_t newsz)
{
#ifdef __cplusplus
    seq_t *newptr = new seq_t[newsz];
    for (size_t i = 0; i < oldsz; ++i)
        newptr[i] = old[i];
    delete[] old;
    return newptr;
#else
    return (seq_t *)realloc(old, newsz * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* static cache for arbitrary call order */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* ensure capacity for first four terms */
    if (cap < 4) {
        size_t needed = (cap == 0) ? 4 : 4;
        if (cap == 0)
            cache = alloc_cache(needed);
        else
            cache = resize_cache(cache, cap, needed);
        /* initialize all newly allocated slots to zero */
        for (size_t i = cap; i < needed; ++i)
            cache[i] = (seq_t)0;
        /* first four values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)32;
        cache[2] = (seq_t)27000;
        cache[3] = (seq_t)24193888;
        cap = 4;
    }

    /* grow cache if needed */
    if ((size_t)n >= cap) {
        size_t newcap = (cap > 1) ? cap * 2 : 4;
        if (newcap <= (size_t)n)
            newcap = n + 1;
        cache = resize_cache(cache, cap, newcap);
        /* initialize new slots to zero */
        for (size_t i = cap; i < newcap; ++i)
            cache[i] = (seq_t)0;
        cap = newcap;
    }

    /* compute missing values using the recurrence */
    /* note: we only recompute values that are not yet known */
    for (idx_t i = 4; (size_t)i < n + 1; ++i) {
        if (cache[i] != (seq_t)0) continue; /* already computed elsewhere */
        cache[i] = (seq_t)928 * cache[i-1]
                 - (seq_t)26942 * cache[i-2]
                 + (seq_t)928 * cache[i-3]
                 - cache[i-4];
    }

    return cache[n];
}
