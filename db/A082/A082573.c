/*
 * A082573  a(1)=1, a(n)=ceiling(n*(a(n-1)+3/a(n-1))).
 * https://oeis.org/A082573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A082573
 * Recurrence: a(1) = 1
 * a(n) = ceil( n * ( a(n-1) + 3 / a(n-1) ) )
 * Equivalent integer form: a(n) = n * a(n-1) + ceil( 3 * n / a(n-1) )
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
    static seq_t *cache = NULL;   /* cached sequence values */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t stored = 0;    /* number of values actually stored */

    /* initialise first term */
    if (stored == 0) {
        ensure_capacity(1, &cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = 1;          /* a(1) = 1 */
        stored = 1;
    }

    if (n <= stored)
        return cache[n - 1];

    /* make room for required terms */
    ensure_capacity(n, &cap, OEISC_VPP(&cache), sizeof(seq_t));

    /* compute missing terms */
    for (idx_t i = stored; i < n; ++i) {
        seq_t prev = cache[i - 1];               /* a(i)  */
        seq_t term = (seq_t)(i + 1) * prev       /* n * a */
                     + ((seq_t)(3 * (i + 1)) + prev - 1) / prev; /* ceil(3n/a) */
        cache[i] = term;
    }

    stored = n;
    return cache[n - 1];
}
