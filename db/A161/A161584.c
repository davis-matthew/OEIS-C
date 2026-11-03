/*
 * A161584  The list of the k values in the common solutions to the 2
 *          equations 13*k+1 = A^2, 17*k+1 = B^2.
 * https://oeis.org/A161584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 224*(a(n+2) - a(n+1)) + a(n), a(1)=0, a(2)=15,
 *            a(3)=3360
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* lazy initialisation of first three terms */
    if (cache_len == 0) {
        ensure_capacity(3, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 15;
        cache[2] = 3360;
        cache_len = 3;
    }

    /* extend cache up to requested n */
    if ((idx_t)n > cache_len) {
        ensure_capacity((size_t)n, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            size_t idx = i - 1;              /* zero‑based index for a(i) */
            seq_t diff = cache[idx - 1] - cache[idx - 2];
            cache[idx] = 224 * diff + cache[idx - 3];
        }
        cache_len = n;
    }

    return cache[n - 1];
}
