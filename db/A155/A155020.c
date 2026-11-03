/*
 * A155020  a(n) = 2*a(n-1) + 2*a(n-2) for n > 2, a(0)=1, a(1)=1, a(2)=3.
 * https://oeis.org/A155020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(n)=2*a(n-1)+2*a(n-2) for n>2
 *            (recurrence)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 1870 (2026-11-03)
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
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        /* space for indices 0,1,2 */
        ensure_capacity(3, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache_len = 3;
    }

    /* extend cache if needed */
    if ((size_t)n >= cache_len) {
        /* ensure capacity up to n+1 */
        ensure_capacity((size_t)n + 1, &cache_cap, OEISC_VPP(&cache),
                        sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev1 = cache[(size_t)(i - 1)];
            seq_t prev2 = cache[(size_t)(i - 2)];
            cache[(size_t)i] = (seq_t)2 * prev1 + (seq_t)2 * prev2;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
