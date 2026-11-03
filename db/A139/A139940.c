/*
 * A139940  Primes of the form 15*x^2+23*y^2.
 * https://oeis.org/A139940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that can be written as 15*x^2 + 23*y^2 with
 *            integer x, y. The representation is tested by brute force,
 *            iterating over y and checking whether (p‑23*y^2)/15 is a square
 * Range:     u8 n <= 4, u16 n <= 419, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9065 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* ----------------------------------------------------------
   Integer square root (floor) for 64‑bit unsigned integers.
   The algorithm uses only integer arithmetic and has O(1) bits
   complexity (at most 32 iterations).  No floating point or
   math.h is required.
   ---------------------------------------------------------- */
static inline uint64_t u64_isqrt(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;          /* highest power of 4 <= x */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ----------------------------------------------------------
   Test whether a prime p can be represented as 15*x^2 + 23*y^2.
   p is guaranteed to be a 64‑bit prime when this is called.
   ---------------------------------------------------------- */
static bool can_represent_prime(uint64_t p)
{
    if (p < 23) return false;   /* smallest possible value is 23 */
    uint64_t max_y = u64_isqrt(p / 23);

    for (uint64_t y = 0; y <= max_y; ++y) {
        __int128 term128 = (__int128)23 * y * y;
        if (term128 > p) break;
        uint64_t term = (uint64_t)term128;
        uint64_t rem  = p - term;

        if (rem % 15) continue;           /* must be divisible by 15 */

        uint64_t q   = rem / 15;          /* q must be a perfect square */
        uint64_t s   = u64_isqrt(q);
        if (s * s == q) return true;     /* representation found */
    }
    return false;
}

/* ----------------------------------------------------------
   The next_prime function returns the smallest prime > x.
   ---------------------------------------------------------- */
#include <stdlib.h>
#include "../utils/next_prime.h"

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

/* ----------------------------------------------------------
   Dynamic array helpers for caching computed terms.
   ---------------------------------------------------------- */

/* ----------------------------------------------------------
   Main sequence function.
   ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Handle the OEIS offset: a(1) is the first term. */
    if (n == 0) return 0;

    static seq_t *cache  = NULL;        /* array of found primes */
    static size_t cache_sz  = 0;        /* number of cached terms   */
    static size_t cache_cap = 0;        /* allocated capacity      */
    static uint64_t last_checked = 1;   /* last prime already examined */

    /* Grow the cache to at least n entries. */
    if ((size_t)n > cache_sz) {
        ensure_capacity((size_t)n, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

        while (cache_sz < (size_t)n) {
            /* Find next prime after the last one examined. */
            last_checked = next_prime(last_checked + 1);

            /* If this prime can be represented, store it. */
            if (can_represent_prime(last_checked))
                cache[cache_sz++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
