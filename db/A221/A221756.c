/*
 * A221756  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal or antidiagonal neighbor, without move-in move-out
 *          straight through or left turns.
 * https://oeis.org/A221756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) + a(n-3) - 4*a(n-4), * recurrence
 *            initial values: a(1)=0, a(2)=2, a(3)=1, a(4)=9
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 205 (2026-11-03)
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

static seq_t *cache = NULL;
static size_t  cap   = 0;
static size_t  len   = 0;

seq_t a(idx_t n)
{
    /* Initialise the base four terms once */
    if (len == 0) {
        /* guarantee space for at least the first four values */
        ensure_capacity(4, &cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)9;
        len = 4;
    }

    /* If the requested value is already computed, return it */
    if (n <= len)
        return cache[n - 1];

    /* Expand cache to hold up to n values */
    ensure_capacity((size_t)n, &cap, OEISC_VPP(&cache), sizeof(seq_t));

    /* Compute terms using the recurrence from len up to n-1 (0‑based) */
    for (idx_t i = len; i < n; ++i) {
        seq_t term = cache[i - 1]                      /* a(n-1) */
                  + (cache[i - 2] * (seq_t)6)          /* + 6*a(n-2) */
                  + cache[i - 3]                      /* + a(n-3) */
                  - (cache[i - 4] * (seq_t)4);        /* - 4*a(n-4) */
        cache[i] = term;
    }

    len = (size_t)n;
    return cache[n - 1];
}
