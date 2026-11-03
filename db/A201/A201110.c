/*
 * A201110  Primes of the form 2n^3+7.
 * https://oeis.org/A201110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2k^3+7 where k ≥ 0 and the value is prime; a(n) is the
 *            n‑th such prime. The OEIS lists a(1)=7, a(2)=23, …
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 164, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Validated: standard, pass, 48 terms through n = 3773 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

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
    /* Static memoization of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of primes stored so far */
    static size_t cache_cap = 0;   /* capacity of the cache */
    static idx_t next_k = 0;       /* next integer k to test (k starts at 0) */

    if (n == 0) {
        return 0;                  /* OEIS offset starts at 1 */
    }

    /* Generate terms until we have at least n entries. */
    while (cache_len < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t f = ((seq_t)2 * k * k * k) + (seq_t)7;  /* 2k³+7 */

        if (isPrimeGeneric(f)) {
            /* Grow the cache if necessary. */
            ensure_capacity(cache_len + 1, &cache_cap,
                            OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_len++] = f;
        }
        next_k++;                   /* advance to next candidate */
    }

    /* cache[0] corresponds to a(1). */
    return cache[n - 1];
}
