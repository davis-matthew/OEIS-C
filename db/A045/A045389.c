/*
 * A045389  Primes congruent to {2, 6} mod 7.
 * https://oeis.org/A045389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 848 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) is the nth prime p such that p % 7 == 2 or p % 7 == 6. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
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
    if (n == 0)
        return 0; /* offset starts at 1; n=0 is not used */

    /* static state that survives across calls */
    static seq_t *cache = NULL;          /* dynamic array of results */
    static size_t cache_cap = 0;         /* current capacity of cache  */
    static idx_t cache_size = 0;         /* number of terms stored   */
    static uint64_t last_prime = 1;      /* last prime examined      */

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        uint64_t p = next_prime(last_prime); /* next prime > last_prime */
        last_prime = p;

        if (p % 7ULL == 2 || p % 7ULL == 6) {
            /* ensure we have room for another element */
            size_t needed = cache_size + 1;
            ensure_capacity(needed, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

            cache[cache_size++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
