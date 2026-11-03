/*
 * A162643  Numbers whose number of divisors is not a power of 2.
 * https://oeis.org/A162643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer m with τ(m) not a power of 2
 *            (τ(m) = number of divisors of m)
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8251 (2026-11-03)
 * Helpers:   is_pow2, mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_pow2.h"
#include "../utils/num_divisors.h"
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
    static seq_t *cache = NULL;          /* stored sequence elements   */
    static size_t len   = 0;             /* current length of cache    */
    static size_t cap   = 0;             /* allocated capacity         */
    static uint64_t cur  = 1;            /* next integer to test       */

    /* generate terms until we have at least n of them */
    while (len < n)
    {
        seq_t d = num_divisors(cur);        /* τ(cur) */
        if (!is_pow2(d))                /* include if τ(cur) is NOT a power of two */
        {
            /* make sure the cache has room for the new element */
            ensure_capacity(len + 1, &cap, OEISC_VPP(&cache), sizeof(seq_t));
            cache[len++] = (seq_t)cur;          /* store cur as seq_t              */
        }
        cur++;                                 /* move to next integer            */
    }

    return cache[n - 1];
}
