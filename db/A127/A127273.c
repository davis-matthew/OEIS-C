/*
 * A127273  Numbers n such that sum of the digits of n and of n+1 divides n +
 *          (n+1), n >= 1.
 * https://oeis.org/A127273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A127273: numbers n such that sum_of_digits(n)+sum_of_digits(n+1)
   divides n+(n+1). This function searches integers in increasing order,
   caching the found terms.  */

#include <stddef.h>                   /* size_t, NULL */
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

/* Helper: sum of decimal digits of an idx_t value. */
static idx_t digit_sum_idx(idx_t x) {
    idx_t s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t sz   = 0;   /* number of cached terms          */
    static size_t cap  = 0;   /* allocated capacity of cache     */
    static idx_t  next = 1;   /* next integer to test (1‑based)   */

    /* Grow the cache until it contains at least n terms */
    while (sz < n) {
        idx_t sum1 = digit_sum_idx(next);
        idx_t sum2 = digit_sum_idx(next + 1);
        idx_t tot  = next + (next + 1);      /* 2*next + 1                 */
        idx_t totSum = sum1 + sum2;

        if (tot % totSum == 0) {
            /* Ensure capacity for the new element */
            ensure_capacity(sz + 1, &cap, OEISC_VPP(&cache), sizeof(seq_t));
            cache[sz++] = (seq_t)next;   /* cast to seq_t – ok for fitting values */
        }
        ++next;
    }

    return cache[n - 1];
}
