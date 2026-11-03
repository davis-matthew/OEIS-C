/*
 * A213169  a(n) = n! + n + 1.
 * https://oeis.org/A213169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! + n + 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 195 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A213169, using a recurrence relation
   a(0) = 2
   a(n) = n * a(n-1) - n * (n-1) + 1   for n ≥ 1
   The function caches computed values so that repeated calls are efficient. */

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

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t size  = 0;  /* number of elements in cache */
    static size_t cap   = 0;  /* allocated capacity for cache */

    /* ensure space for value at index n */
    if (n + 1 > size) {
        /* expand cache up to n */
        ensure_capacity(n + 1, &cap,
                        OEISC_VPP(&cache), sizeof(seq_t));

        /* base case: compute a(0) if not yet present */
        if (size == 0) {
            cache[0] = 2;          /* 0! + 0 + 1 = 2 */
            size = 1;
        }

        /* compute remaining terms up to n */
        for (idx_t i = size; i <= n; ++i) {
            seq_t term = (seq_t)i * cache[i - 1];
            term -= (seq_t)i * (i - 1);
            term += 1;
            cache[i] = term;
        }
        size = n + 1;  /* cache now contains [0..n] */
    }

    return cache[n];
}
