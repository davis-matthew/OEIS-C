/*
 * A200562  Expansion of 1 / ((1 - 2*x) * (1 + 3*x + 4*x^2)) in powers of x.
 * https://oeis.org/A200562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 2*a(n-2) + 8*a(n-3), with initial values
 *            a(0)=1, a(1)=-1, a(2)=3
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 30, i64 n <= 64, i128 n <= 126,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 2298 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
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
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of values already computed */
    static size_t cache_cap = 0;   /* allocated capacity (in seq_t units) */
    idx_t i;

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Ensure the array has enough space for indices up to n */
    if ((idx_t)cache_cap <= n) {
        /* Grow capacity: double until it exceeds n + 1 */
        size_t new_cap = cache_cap > 0 ? cache_cap << 1 : 4;
        while ((idx_t)new_cap <= n)
            new_cap <<= 1;
        ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache),
                         sizeof(seq_t));
    }

    /* Compute values from cache_len to n (inclusive) */
    for (i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0)          val = 1;
        else if (i == 1)     val = -1;
        else if (i == 2)     val = 3;
        else
            val = -cache[i-1] + (seq_t)2 * cache[i-2] + (seq_t)8 * cache[i-3];
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
