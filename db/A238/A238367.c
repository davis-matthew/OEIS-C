/*
 * A238367  Even numbers n such that n is not divisible by 2^d where d is the
 *          number of distinct odd prime factors of n.
 * https://oeis.org/A238367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth even number that is NOT divisible by 2^d, where
 *            d is the number of distinct odd prime factors of that number
 * Range:     u8 n <= 26, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-03)
 * Helpers:   mem, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/omega.h"
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
    /* Basic sanity: the sequence is defined for n >= 1 */
    if (n == 0) {
        /* Undefined behaviour for offset < 1; return 0 */
        return (seq_t)0;
    }

    /* Static state to remember the cache of found terms and the
       last checked even candidate.  The cache contains the
       sequence values in order.  All accesses are protected
       against overshoot by ensure_capacity. */
    static seq_t *cache = NULL;          /* array of found terms            */
    static size_t cache_cap = 0;        /* current capacity                */
    static size_t cache_len = 0;        /* number of terms stored          */
    static uint64_t last_checked = 0;   /* last even number inspected      */

    /* Grow the cache until we have at least n terms */
    while (cache_len < n) {
        uint64_t cand = last_checked + 2;           /* next even candidate    */
        last_checked = cand;

        /* Count distinct primes of cand (includes 2 because cand is even) */
        int d_total = omega(cand);
        int d_odd   = d_total - 1;                  /* subtract prime 2      */

        /* Compute the exponent of 2 in cand   (v2) */
        unsigned v2 = 0;
        uint64_t temp = cand;
        while ((temp & 1) == 0) {
            ++v2;
            temp >>= 1;
        }

        /* Include cand iff v2 < d_odd  (i.e., 2^d_odd does NOT divide cand) */
        if (v2 < (unsigned)d_odd) {
            /* Ensure there is space for another element */
            ensure_capacity(cache_len + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_len++] = (seq_t)cand;
        }
    }

    /* Return the (n-1)-th element (offset 1) */
    return cache[n - 1];
}
