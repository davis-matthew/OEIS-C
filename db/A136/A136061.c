/*
 * A136061  Primes p such that (p+4)/5 is also prime.
 * https://oeis.org/A136061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that (p+4)/5 is also prime
 * Range:     u8 n <= 6, u16 n <= 269, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 940 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *cache = NULL;          /* cached terms of the sequence */
static size_t cache_len = 0;         /* number of cached terms */
static size_t cache_cap = 0;         /* capacity of the cache array */
static uint64_t next_q = 2;          /* next candidate for q = (p+4)/5 */

/* Append a term to the cache, growing it if necessary */
static void append_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap * 2;
        ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Return the n-th term (n >= 1) of sequence A136061 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range, undefined */

    /* build terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Find the next prime q */
        while (!is_prime(next_q)) {
            next_q += (next_q % 2 == 0 ? 1 : 2);
        }
        uint64_t q = next_q;
        /* Compute candidate p = 5*q - 4 */
        uint64_t p = 5 * q - 4;
        if (is_prime(p)) {
            append_term((seq_t)p);
        }
        /* Advance to the next odd candidate */
        next_q += (next_q % 2 == 0 ? 1 : 2);
    }

    return cache[n - 1];
}
