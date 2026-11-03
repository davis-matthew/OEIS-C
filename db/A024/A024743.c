/*
 * A024743  a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *          a(1)=3, a(2)=1, and a(3)=2.
 * https://oeis.org/A024743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..⌊(n+1)/4⌋} a(2*i-1) * a(n-2*i+1), with a(1)=3,
 *            a(2)=1, a(3)=2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size) /* elem_size: sizeof(T) */
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, new_cap);
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
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* static array to remember which indices are already initialized */
    static uint8_t *init = NULL;
    static size_t init_cap = 0;
    static size_t done = 0;   /* largest index whose value is known */

    /* Ensure memory for both arrays */
    ensure_cache(n, &cache, &cache_cap);

    size_t old_init_cap = init_cap;
    ensure_capacity(n + 1, &init_cap, OEISC_VPP(&init), sizeof(uint8_t));
    /* Zero the newly allocated part of init array */
    for (size_t i = old_init_cap; i <= n; ++i)
        init[i] = 0;

    /* Compute missing values in increasing order */
    for (idx_t k = done + 1; k <= n; ++k)
    {
        if (k == 1)
            cache[1] = (seq_t)3;
        else if (k == 2)
            cache[2] = (seq_t)1;
        else if (k == 3)
            cache[3] = (seq_t)2;
        else
        {
            seq_t sum = 0;
            idx_t m = (k + 1) / 4;          /* ⌊(k+1)/4⌋ */
            for (idx_t i = 1; i <= m; ++i)
            {
                idx_t j1 = 2 * i - 1;
                idx_t j2 = k - 2 * i + 1;
                sum += cache[j1] * cache[j2];
            }
            cache[k] = sum;
        }
        init[k] = 1;
    }
    done = n;

    return cache[n];
}
