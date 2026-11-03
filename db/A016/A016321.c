/*
 * A016321  Expansion of 1/((1-2x)*(1-9x)*(1-10x)).
 * https://oeis.org/A016321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 90*a(n-2) + 2^n (n >= 2), a(0) = 1, a(1) = 21
 *            (expansion of 1/((1-2x)*(1-9x)*(1-10x))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 853 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"
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

static seq_t *cache = NULL;
static size_t cache_len   = 0;  /* number of filled terms in cache   */
static size_t cache_cap   = 0;  /* allocated capacity                 */

seq_t a(idx_t n)
{
    /* If already computed, return cached value. */
    if (cache_len > (size_t)n)
        return cache[(size_t)n];

    /* Ensure we have enough space. */
    size_t needed = (size_t)n + 1;
    ensure_capacity(needed, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

    /* Compute missing terms. */
    for (size_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;
        }
        else if (i == 1)
        {
            cache[1] = (seq_t)21;
        }
        else
        {
            seq_t term = 19 * cache[i-1] - 90 * cache[i-2] + pow2((idx_t)i);
            cache[i] = term;
        }
    }

    cache_len = needed;
    return cache[(size_t)n];
}
