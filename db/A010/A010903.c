/*
 * A010903  Pisot sequence E(3,13): a(n) = floor(a(n-1)^2/a(n-2) + 1/2).
 * https://oeis.org/A010903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2), a(0) = 3, a(1) = 13
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 925 (2026-11-03)
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
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Initialize cache with first two terms */
    if (cache_len == 0) {
        ensure_capacity(2, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)13;
        cache_len = 2;
    }

    /* Return already computed value */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Allocate space for the required terms */
    size_t target_len = (size_t)n + 1ULL;
    if (target_len > cache_cap)
        ensure_capacity(target_len, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

    /* Compute missing terms using the linear recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t prevprev = cache[i - 2];
        seq_t prev     = cache[i - 1];
        seq_t val =   (seq_t)5 * prev
                    - (seq_t)3 * prevprev;
        cache[i] = val;
    }
    cache_len = target_len;   /* update cached length */

    return cache[(size_t)n];
}
