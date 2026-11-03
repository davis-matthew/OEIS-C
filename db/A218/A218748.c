/*
 * A218748  a(n) = (45^n - 1)/44.
 * https://oeis.org/A218748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (45^n - 1)/44 Recurrence: a(0)=0, a(n)=45*a(n-1)+1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 596 (2026-11-03)
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

static seq_t *cache = NULL;
static size_t cap   = 0;   /* allocated capacity, size_t matches ensure_capacity */
static idx_t len   = 0;    /* number of stored terms */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (len == 0) {
        ensure_capacity(1, &cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = (seq_t)0;
        len = 1;
    }

    /* return cached value if available */
    if (n < len)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t val = cache[i-1] * (seq_t)45 + (seq_t)1;
        ensure_capacity(len + 1, &cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[len] = val;
        ++len;
    }

    return cache[n];
}
