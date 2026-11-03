/*
 * A142244  Primes congruent to 23 mod 42.
 * https://oeis.org/A142244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that p % 42 == 23 (OEIS A142244)
 * Range:     u8 n <= 5, u16 n <= 552, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 952 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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
    /* static memoization */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_sz  = 0;
    static idx_t   next_k    = 0;  /* k index for the next candidate: p = 42*k + 23 */

    if (n == 0) return 0;          /* valid indices start at 1 */

    while (cache_sz < n) {
        idx_t candidate = 42 * next_k + 23;
        if (is_prime(candidate)) {
            /* grow storage if needed */
            ensure_capacity(cache_sz + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_sz++] = (seq_t)candidate;
        }
        next_k++;
    }

    return cache[n - 1];
}
