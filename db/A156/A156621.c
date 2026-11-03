/*
 * A156621  5-almost primes of the form p^2 - 2, where p is prime.
 * https://oeis.org/A156621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-03)
 * Helpers:   factor_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = p^2–2 for the nth prime p such that p^2–2 has exactly
 * five prime factors (counted with multiplicity). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
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
    /* zero‑based index */
    if (n == 0) return (seq_t)0;

    /* static storage for results */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_p = 2;   /* next prime to test */

    /* grow cache until we have n entries */
    while (cache_len < n)
    {
        uint64_t p = next_p;

        /* value p^2 - 2 */
        seq_t val = (seq_t)p * (seq_t)p - (seq_t)2;
        uint64_t val64 = (uint64_t)val;  /* for factorization */

        /* factorization */
        uint64_t pr[32], ex[32];
        size_t cnt;
        factor_u64(val64, pr, ex, &cnt);

        /* compute total number of prime factors (with multiplicity) */
        uint64_t total = 0;
        for (size_t i = 0; i < cnt; ++i)
            total += ex[i];

        if (total == 5)
        {
            /* store if it is a 5‑almost prime */
            ensure_capacity(cache_len + 1, &cache_cap,
                            OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_len++] = val;
        }

        /* advance to the next prime */
        next_p = next_prime(p + 1);
    }

    return cache[n - 1];
}
