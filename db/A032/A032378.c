/*
 * A032378  Noncubes k that are divisible by floor(k^(1/3)).
 * https://oeis.org/A032378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 853, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9674 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032378: Noncubes k that are divisible by floor(k^(1/3)). */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
    if (n == 0)         /* offset is 1; undefined for n < 1 */
        return 0;

    /* static cache of already-found terms */
    static seq_t *terms  = NULL;   /* array of found k values     */
    static idx_t  used   = 0;      /* number of terms stored     */
    static idx_t  cap    = 0;      /* capacity of the array      */
    static seq_t  next_k = 1;      /* next integer to test       */

    /* return cached value if already available */
    if (used >= n)
        return terms[n - 1];

    /* generate terms until the requested index is reached */
    while (used < n) {
        seq_t rc = floor_cube_root(next_k);

        /* skip perfect cubes */
        if (rc * rc * rc != next_k) {
            /* check divisibility */
            if (next_k % rc == 0) {
                /* grow the cache if necessary */
                ensure_capacity(used + 1, &cap,
                                 OEISC_VPP(&terms), sizeof(seq_t));
                terms[used++] = next_k;
            }
        }
        ++next_k;
    }

    return terms[n - 1];
}
