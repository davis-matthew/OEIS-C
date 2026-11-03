/*
 * A056889  Numerators of continued fraction for left factorial.
 * https://oeis.org/A056889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 21, i32 n <= 31, i64 n <= 45, i128 n <= 73,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 816 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(1)=1;
   a(2*n)=n*a(2*n-1)+a(2*n-2);
   a(2*n+1)=-a(2*n)+a(2*n-1); */

#include <stdlib.h>         /* malloc, free */
#include <stdint.h>         /* NULL */
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
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;      /* allocated capacity (elements)   */
    static size_t  len    = 0;      /* initialized elements above      */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 2;                       /* at least a(0) and a(1) */
        cache = mem_new(seq_t, cap);
        if (cache == NULL) abort();    /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        len = 2;
    }

    /* extend cache if required */
    if ((idx_t)len <= n) {
        /* ensure capacity for indices 0 … n */
        ensure_capacity((size_t)(n + 1), &cap, OEISC_VPP(&cache),
                         sizeof(seq_t));

        /* compute missing terms */
        for (idx_t i = (idx_t)len; i <= n; ++i) {
            if ((i & 1ULL) == 0ULL) {          /* even index: 2*n */
                seq_t k   = (seq_t)(i >> 1UL); /* n = i/2          */
                cache[i] = k * cache[i - 1] + cache[i - 2];
            } else {                          /* odd index: 2*n+1 */
                cache[i] = -cache[i - 1] + cache[i - 2];
            }
        }
        len = (size_t)(n + 1);
    }

    return cache[n];
}
