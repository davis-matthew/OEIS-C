/*
 * A138634  Nonnegative integers k such that 17*k-9 is prime.
 * https://oeis.org/A138634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 2443, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9491 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th non‑negative integer k with 17*k-9 prime
   (OEIS A138634).  a(1)=4, a(2)=8, ... */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_k = 1;   /* candidate k to test next */

    if (n == 0) return 0;      /* offset starts at 1 */

    /* Extend cache until it contains at least n terms */
    while (cache_len < n) {
        /* Find the next k such that 17*k-9 is prime */
        while (1) {
            seq_t val = (seq_t)17 * (seq_t)next_k - (seq_t)9;
            if (is_prime((idx_t)val)) {
                /* store k in the cache */
                ensure_capacity(cache_len + 1, &cache_cap, OEISC_VPP(&cache),
                                sizeof(seq_t));
                cache[cache_len++] = (seq_t)next_k;
                ++next_k;  /* move to the next candidate for future calls */
                break;
            }
            ++next_k;
        }
    }

    return cache[n - 1];
}
