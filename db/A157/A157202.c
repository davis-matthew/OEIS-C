/*
 * A157202  Numbers k such that 66*k + 5 is prime.
 * https://oeis.org/A157202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 81 terms through n = 962 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 66*k + 5 is prime.
   Computed by brute‑force search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
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
    /* Static cache of found terms */
    static seq_t *cache = NULL;    /* array of k values */
    static size_t cache_size = 0;  /* number of cached terms */
    static size_t cache_capacity = 0;

    /* If the requested term is already cached, return it */
    if (n <= cache_size && n >= 1) {
        return cache[n - 1];
    }

    /* Start searching from the next candidate after the last cached k */
    idx_t k = (cache_size == 0) ? 0 : (idx_t)cache[cache_size - 1] + 1;

    /* Find terms until we reach the n‑th one */
    while (cache_size < (size_t)n) {
        /* 66*k + 5 will always fit in idx_t for the n values required
           (n ≤ 100 000 → k < 10⁶ → 66*k+5 < 7×10⁷ < 2⁶⁴).      */
        idx_t val = 66 * k + 5;

        /* Check primality using the generic 64‑bit test */
        if (is_prime(val)) {
            /* Grow the cache if necessary */
            if (cache_size == cache_capacity) {
                size_t new_cap = (cache_capacity == 0) ? 8 : cache_capacity * 2;
                ensure_capacity(new_cap, &cache_capacity, OEISC_VPP(&cache),
                                sizeof(seq_t));
            }
            /* Store the found k */
            cache[cache_size++] = (seq_t)k;
        }

        ++k;
    }

    /* Return the requested term, which is now cached */
    return cache[n - 1];
}
