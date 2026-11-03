/*
 * A048909  9-gonal (or nonagonal) triangular numbers.
 * https://oeis.org/A048909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 255*a(n-1) - 255*a(n-2) + a(n-3) for n >= 4 Initial
 *            values: a(1) = 1 a(2) = 325 a(3) = 82621
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 376 (2026-11-03)
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

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary call order */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;    /* largest n computed (0 means none yet) */

    if (n == 0) {
        return 0;                    /* offset is 1, return 0 for n=0 */
    }

    /* ensure capacity for indices up to n (1‑based) */
    if (max_computed < n) {
        ensure_capacity((size_t)n + 1, &cache_cap, OEISC_VPP(&cache),
                        sizeof(seq_t));

        /* compute missing terms */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = 1;
            } else if (i == 2) {
                cache[i] = 325;
            } else if (i == 3) {
                cache[i] = 82621;
            } else {
                /* recurrence a(n) = 255*a(n-1) - 255*a(n-2) + a(n-3) */
                cache[i] = 255 * cache[i - 1] - 255 * cache[i - 2] + cache[i - 3];
            }
        }
        max_computed = n;
    }
    return cache[n];
}
