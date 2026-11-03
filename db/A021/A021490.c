/*
 * A021490  Decimal expansion of 1/486.
 * https://oeis.org/A021490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth digit after the decimal point in the expansion of
 *            1/486. Computed by iteratively maintaining the remainder r of r
 *            ≡ 1 × 10^k (mod 486)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static idx_t cache_len = 0;            /* number of digits stored      */
static size_t cache_cap = 0;           /* current capacity of cache    */
static seq_t *cache = NULL;            /* array of computed digits     */
static seq_t rem = 1;                  /* remainder after last digit   */

/* Ensure that digits up to index N are computed and stored */
static void ensure_upto(idx_t N)
{
    if (N < cache_len)
        return;

    /* Grow capacity if necessary */
    size_t needed = (size_t)N + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
    }

    /* Compute missing digits */
    for (idx_t i = cache_len; i <= N; ++i) {
        seq_t digit = (rem * (seq_t)10) / (seq_t)486;
        rem       = (rem * (seq_t)10) % (seq_t)486;
        cache[i]  = digit;
    }
    cache_len = (idx_t)N + 1;
}

/* External function requested by the build system */
seq_t a(idx_t n)
{
    ensure_upto(n);
    return cache[n];
}
