/*
 * A023260  Primes that remain prime through 2 iterations of function f(x) =
 *          8x + 1.
 * https://oeis.org/A023260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are primes p such that 8p+1 and 64p+9 are also primes
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 917 (2026-11-03)
 * Helpers:   is_prime, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(idx_t n) returns the n-th element of the sequence.
   It keeps a static cache of found terms and extends it lazily on
   demand.  The search proceeds by testing consecutive integers for
   primality via the build‑supplied helper functions */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
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
    /* static variables retain their values between calls */
    static size_t cache_size = 0;            /* number of terms cached     */
    static size_t cache_cap  = 0;            /* capacity of cache array    */
    static seq_t *cache        = NULL;       /* array of cached terms      */
    static idx_t last_checked  = 1;          /* last integer examined      */

    while (cache_size < n) {
        last_checked++;                     /* next integer to test       */
        if (!is_prime(last_checked))        /* skip non‑primes            */
            continue;

        /* cast to the build’s integer type for the arithmetic */
        seq_t p  = (seq_t)last_checked;
        seq_t f1 = (seq_t)8 * p + 1;
        seq_t f2 = (seq_t)64 * p + 9;

        if (isPrimeGeneric(f1) && isPrimeGeneric(f2)) {
            /* grow the cache if necessary */
            ensure_capacity(cache_size + 1, &cache_cap,
                            OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_size++] = p;
        }
    }
    return cache[n - 1];
}
