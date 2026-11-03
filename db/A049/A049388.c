/*
 * A049388  a(n) = (n+7)!/7!.
 * https://oeis.org/A049388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+7)! / 7! (equivalently, a(0)=1 and a(n)=a(n-1)*(n+7)
 *            for n>0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 294 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* seq_t and idx_t are defined by the build system.  The function returns the
   nth term of OEIS A049388.  It memoizes results so repeated calls for any
   n are efficient. */
seq_t a(idx_t n)
{
    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* current allocated capacity (in elements)     */
    static idx_t  last   = 0;   /* highest index computed so far                */

    /* Initialise cache on the first call.  cache[0] = 1 according to the formula. */
    if (cache == NULL) {
        /* Allocate space for at least one element. */
        ensure_capacity(1, &cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        last = 0;
    }

    /* If the requested n is already cached, return the stored value. */
    if (n <= last)
        return cache[n];

    /* Need to compute values for indices last+1 .. n.  Ensure the array is large enough. */
    if (n >= cap)
        ensure_capacity((size_t)n + 1, &cap, OEISC_VPP(&cache), sizeof(seq_t));

    /* Compute the remaining values using the recurrence a(k) = a(k-1)*(k+7). */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t mult = (seq_t)(k + 7);                     /* small integer cast to seq_t */
        cache[k] = seq_mul(cache[k - 1], mult);          /* a(k) = a(k-1) * (k+7) */
    }

    last = n;  /* Update the highest computed index. */
    return cache[n];
}
