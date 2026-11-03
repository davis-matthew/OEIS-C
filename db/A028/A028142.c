/*
 * A028142  Expansion of 1/((1-4x)(1-6x)(1-10x)(1-11x)).
 * https://oeis.org/A028142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (24*11^(n+3) - 35*10^(n+3) + 21*6^(n+3) - 10*4^(n+3))/840
 *            * Equivalently a linear recurrence of order 4: * a(0)=1,
 *            a(1)=31, a(2)=617, a(3)=10067 * a(n)=31*a(n-1) - 344*a(n-2) +
 *            1604*a(n-3) - 2640*a(n-4) for n>=4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size) /* elem_size: sizeof(T) */
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, new_cap);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    /* cache the sequence to allow random access */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements stored so far */
    static idx_t cache_cap = 0;   /* allocated capacity */
    static const seq_t init_vals[4] = {1, 31, 617, 10067};

    /* handle small n directly */
    if (n < 4)
        return init_vals[n];

    /* initialise cache if needed */
    if (cache == NULL) {
        cache_cap = 4;
        cache_len  = 4;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return init_vals[n]; /* fallback on allocation failure */
        for (idx_t i = 0; i < 4; ++i)
            cache[i] = init_vals[i];
    }

    /* grow cache if request is beyond current capacity */
    if (n + 1 > cache_cap) {
        ensure_capacity(n + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
    }

    /* compute new terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term = 31 * cache[i - 1] - 344 * cache[i - 2] +
                     1604 * cache[i - 3] - 2640 * cache[i - 4];
        cache[i] = term;
    }
    cache_len = n + 1;  /* update stored length */

    return cache[n];
}
