/*
 * A142574  Primes congruent to 44 mod 53.
 * https://oeis.org/A142574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p % 53 == 44
 * Range:     u8 n <= 1, u16 n <= 129, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 990 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_cap   = 0;
    static size_t cache_size  = 0;
    static uint64_t next_start = 1;   /* next number to search from (exclusive) */

    while (cache_size < n) {
        uint64_t prime;

        /* find the next prime that satisfies p % 53 == 44 */
        do {
            prime = next_prime(next_start);   /* next prime > next_start */
            next_start = prime + 1;           /* start searching after this prime */
        } while ((prime % 53ULL) != 44ULL);

        /* grow cache if necessary */
        ensure_capacity(cache_size + 1, &cache_cap,
                        OEISC_VPP(&cache), sizeof(seq_t));

        cache[cache_size++] = (seq_t)prime;
    }

    return cache[n - 1];
}
