/*
 * A039304  Number of distinct quadratic residues mod 7^n.
 * https://oeis.org/A039304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7^n + 3)*7/16). Recurrence: a(0)=1, a(1)=4,
 *            a(2)=22, a(n)=7*a(n-1)+a(n-2)-7*a(n-3) for n≥3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 200 (2026-11-03)
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

static seq_t *cache = NULL;   /* memoization buffer */
static size_t   cap   = 0;     /* allocated size of cache */
static idx_t    last  = 0;     /* index of last computed element */

seq_t a(idx_t n)
{
    /* Directly return the first three values to avoid allocation. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)22;

    /* Grow cache if necessary. */
    if (n + 1 > cap) {
        ensure_capacity(n + 1, &cap, OEISC_VPP(&cache), sizeof(seq_t));
    }

    /* Initialise the cache on the first real call. */
    if (last == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)22;
        last    = 2;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t v1 = cache[i - 1] * (seq_t)7;
        seq_t v2 = cache[i - 2];
        seq_t v3 = cache[i - 3] * (seq_t)7;
        cache[i] = (v1 + v2) - v3;
    }

    last = n;
    return cache[n];
}
