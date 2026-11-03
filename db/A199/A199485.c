/*
 * A199485  a(n) = 8*7^n+1.
 * https://oeis.org/A199485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*7^n + 1 * Recurrence: a(0) = 9, a(n) = 7*a(n-1) - 6 for
 *            n > 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 963 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_used = 0;   /* number of elements currently filled */

    /* initialise cache with a(0) on first call */
    if (cache_used == 0) {
        ensure_capacity(1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = (seq_t)9;         /* a(0) = 9 */
        cache_used = 1;
    }

    /* grow cache up to n if needed */
    if ((idx_t)cache_used <= n) {
        /* extend storage if necessary */
        ensure_capacity((size_t)(n + 1), &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

        /* compute missing terms using the recurrence */
        for (idx_t i = cache_used; i <= n; ++i) {
            cache[i] = (seq_t)7 * cache[i - 1] - (seq_t)6;
        }
        cache_used = (size_t)(n + 1);
    }

    return cache[n];
}
