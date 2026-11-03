/*
 * A008345  a(n+1) = a(n)-b(n+1) if a(n) >= b(n+1) else a(n)+b(n+1), where
 *          {b(n)} are the triangular numbers A000217.
 * https://oeis.org/A008345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(n+1) = (a(n) >= T(n+1)) ? a(n)-T(n+1) : a(n)+T(n+1),
 *            where T(k) = k*(k+1)/2
 * Range:     u8 n <= 15, u16 n <= 274, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 72 terms through n = 7656 (2026-11-03)
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
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static size_t cache_cap = 0;

    if (cache_size == 0) {
        ensure_capacity(1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = 0;
        cache_size = 1;
    }

    if (n < cache_size)
        return cache[n];

    ensure_capacity(n + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        idx_t tri_idx = i;
        seq_t tri = (seq_t)tri_idx * ((seq_t)tri_idx + 1) / 2;
        cache[i] = (prev >= tri) ? prev - tri : prev + tri;
    }
    cache_size = n + 1;
    return cache[n];
}
