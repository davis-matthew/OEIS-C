/*
 * A141858  Primes congruent to 2 mod 13.
 * https://oeis.org/A141858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 539, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 952 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   OEIS A141858: primes congruent to 2 (mod 13)  */
/*   a(n)  =  the nth prime p such that p % 13 == 2   */
#include <stddef.h>
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

static seq_t *cache = NULL;       /* cached terms */
static size_t cache_size = 0;     /* number of cached terms */
static size_t cache_cap   = 0;     /* allocated capacity of cache */

seq_t a(idx_t n)
{
    /* The function works for any value of n >= 1.  Calls may arrive
       in arbitrary order; the cache grows lazily only when needed.  */

    if (n == 0) {          /* safety guard – the sequence is 1‑based */
        return 0;
    }

    /* Grow the cache until it contains at least n terms */
    if (cache_size < (size_t)n) {
        idx_t k;                      /* index of the current multiple of 13 */
        if (cache_size == 0) {
            k = 0;                    /* candidate = 13*0+2 = 2 */
        } else {
            /* Compute k for the first candidate after the last cached prime */
            k = ((idx_t)cache[cache_size - 1] - 2) / 13 + 1;
        }

        while (cache_size < (size_t)n) {
            idx_t candidate = 13 * k + 2;
            if (is_prime(candidate)) {                    /* test primality */
                ensure_capacity(cache_size + 1, &cache_cap,
                                OEISC_VPP(&cache), sizeof(seq_t));
                cache[cache_size++] = (seq_t)candidate;   /* store in cache */
            }
            ++k;   /* next candidate 13*j+2 */
        }
    }

    return cache[n - 1];
}
