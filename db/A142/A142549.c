/*
 * A142549  Primes congruent to 19 mod 53.
 * https://oeis.org/A142549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p with p ≡ 19 (mod 53)
 * Range:     u8 n <= 1, u16 n <= 124, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 866 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

/* Helper: ensure that a dynamic array has at least 'needed' slots */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    *ptr = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}
#endif

/* Cached list of primes ≡ 19 (mod 53) */
static seq_t *prime_cache = NULL;
static size_t prime_cache_size = 0;
static size_t prime_cache_cap  = 0;
static idx_t next_k = 0;  /* k such that candidate = 19 + 53*k */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1 */
    if (n < 1) return 0;

    /* Fill cache until we have at least n primes */
    while (prime_cache_size < n) {
        seq_t candidate = (seq_t)19 + (seq_t)53 * (seq_t)next_k;
        if (isPrimeGeneric((seq_t)candidate)) {
            /* add to cache */
            ensure_capacity(prime_cache_size + 1, &prime_cache_cap,
                            OEISC_VPP(&prime_cache), sizeof(seq_t));
            prime_cache[prime_cache_size++] = candidate;
        }
        ++next_k;
    }

    return prime_cache[n - 1];
}
