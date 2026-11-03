/*
 * A030038  a(n+1) = Sum_{k=0..floor(3*n/5)} a(k) * a(n-k).
 * https://oeis.org/A030038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 54, u128 n <= 102,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 991 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n+1) = Σ_{k=0..⌊3n/5⌋} a(k) * a(n-k) */
#include <stddef.h>
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

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* initialise first term */
    if (cache_len == 0) {
        /* allocate space for at least one element */
        ensure_capacity(1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        ensure_capacity((size_t)n + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
    }

    /* compute missing terms */
    for (idx_t m = cache_len; m <= n; ++m) {
        idx_t limit = (idx_t)((3ull * (m - 1ull)) / 5ull);  /* floor(3*(m-1)/5) */
        seq_t sum = 0;
        for (idx_t k = 0; k <= limit; ++k) {
            sum += cache[k] * cache[m - 1 - k];
        }
        cache[m] = sum;
    }
    cache_len = n + 1;  /* we now have terms up to n */

    return cache[n];
}
