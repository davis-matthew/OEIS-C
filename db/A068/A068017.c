/*
 * A068017  Composite n such that sigma(n) - 1 and sigma(n) + 1 are twin
 *          primes.
 * https://oeis.org/A068017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 985 (2026-11-03)
 * Helpers:   is_prime, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A068017: composite n such that sigma(n)-1
   and sigma(n)+1 are twin primes. The function searches for the
   required terms and caches them for subsequent calls. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime.h"
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

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
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

static seq_t *cache = NULL;   /* cached sequence values          */
static size_t cache_size = 0; /* number of cached terms          */
static size_t cache_cap = 0;  /* allocated cache capacity        */
static idx_t next_candidate = 4; /* next integer to test (composite) */

seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined for n=0, return 0 */
        return 0;

    if (n <= cache_size)            /* already computed */
        return cache[n - 1];

    /* Generate further terms until we have at least n of them */
    while (cache_size < n)
    {
        /* Skip primes – only composites are relevant */
        if (is_prime(next_candidate))
        {
            ++next_candidate;
            continue;
        }

        /* Compute sigma(n) */
        seq_t sigma = sigma_of(next_candidate);

        /* Check twin‑prime condition on sigma-1 and sigma+1 */
        seq_t s_minus = sigma - 1;
        seq_t s_plus  = sigma + 1;

        if (isPrimeGeneric(s_minus) && isPrimeGeneric(s_plus))
        {
            /* Ensure cache has room for another element */
            ensure_capacity(cache_size + 1, &cache_cap,
                            OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_size++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
