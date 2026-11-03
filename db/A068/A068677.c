/*
 * A068677  Numbers that yield primes when a 7 is prefixed or suffixed to
 *          them.
 * https://oeis.org/A068677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1246, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9936 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A068677: numbers n such that both 7n and n7 are prime numbers. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

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

/* static storage for memoized terms */
static seq_t *cache = NULL;
static size_t  cache_len  = 0;
static size_t  cache_cap  = 0;
static idx_t   next_candidate = 1;  /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0) {  /* behavior below the offset does not matter */
        return 0;
    }

    /* Return cached value if already computed */
    if (cache_len >= (size_t)n) {
        return cache[n - 1];
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        idx_t c = next_candidate++;

        /* quick sieving by small divisibility */
        if ((c & 1) == 0)                continue; /* even */
        if (c % 3 == 2)                  continue; /* would make both concatenations divisible by 3 */
        if (c % 5 == 0)                  continue; /* would make the prefixed concatenation divisible by 5 */

        /* construct the prefixes/suffixes */
        seq_t left  = concat_parts(7, c);   /* 7 followed by c */
        seq_t right = concat_parts(c, 7);   /* c followed by 7 */

        /* test primality */
        if (isPrimeGeneric(left) && isPrimeGeneric(right)) {
            /* ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
            }
            cache[cache_len++] = c;
        }
    }

    return cache[n - 1];
}
