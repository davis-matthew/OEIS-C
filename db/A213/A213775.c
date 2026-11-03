/*
 * A213775  Principal diagonal of the convolution array A213774.
 * https://oeis.org/A213775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 4*a(n-4) - a(n-6)
 *            Initial values: a(1)=1, a(2)=11, a(3)=38, a(4)=97, a(5)=213,
 *            a(6)=432
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 35, u64 n <= 80, u128 n <= 170,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 978 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Base values for the first six terms */
    const seq_t base[6] = {(seq_t)1, (seq_t)11, (seq_t)38,
                           (seq_t)97, (seq_t)213, (seq_t)432};

    /* Return directly for the first six entries */
    if (n <= 6)
        return base[n - 1U];

    /* Initialise cache on the first call */
    if (cache == NULL) {
        ensure_capacity(6, &cap, OEISC_VPP(&cache), sizeof(seq_t));
        for (size_t i = 0; i < 6; ++i)
            cache[i] = base[i];
        len = 6;
    }

    /* Grow cache if needed to hold up to index n */
    if ((size_t)n > cap) {
        size_t newcap = cap ? cap << 1 : 16;
        if (newcap < (size_t)n)
            newcap = (size_t)n;
        ensure_capacity(newcap, &cap, OEISC_VPP(&cache), sizeof(seq_t));
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)len + 1U; i <= n; ++i) {
        /* a(i) uses a(i-1), a(i-2), a(i-3), a(i-4), a(i-6) */
        seq_t term = 4 * cache[i - 2U]            /* 4*a(i-1) */
                  - 4 * cache[i - 3U]            /* -4*a(i-2) */
                  - 2 * cache[i - 4U]            /* -2*a(i-3) */
                  + 4 * cache[i - 5U]            /* +4*a(i-4) */
                  -      cache[i - 7U];          /* -a(i-6) */
        cache[i - 1U] = term;
    }

    len = (size_t)n;
    return cache[n - 1U];
}
