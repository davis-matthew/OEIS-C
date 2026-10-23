/*
 * A135334  Number of Dyck paths of semilength n having no UDDU's starting at
 *          level 1.
 * https://oeis.org/A135334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of Dyck paths of semilength n having no UDDU's
 *            starting at level 1. Recurrence (valid for n >= 4): 2*(n+1)*a(n)
 *            + 2*(1-5n)*a(n-1) + 3*(3n-1)*a(n-2) + 2*(1-2n)*a(n-3) = 0
 *            Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=4. Implementation
 *            uses the recurrence with 128‑bit intermediate arit...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#if defined(__cplusplus)
# include <type_traits>
#endif

/* Helper: compute a(i) for i >= 4 using recurrence.
   For integral seq_t types a 128‑bit unsigned intermediate is used to avoid overflow.
   For non‑integral seq_t (big‑integer) native multiplication and division are used. */
static inline seq_t recurrence_term(idx_t i, seq_t *cache)
{
#if defined(__cplusplus)
    if constexpr (std::is_integral<seq_t>::value) {
        /* 128‑bit arithmetic for uint64_t, uint32_t etc. */
        __uint128_t co1 = (2ULL * (5ULL * i - 1ULL));
        __uint128_t co2 = (2ULL * (2ULL * i - 1ULL));
        __uint128_t co3 = (3ULL * (3ULL * i - 1ULL));
        __uint128_t t1   = (__uint128_t)cache[i-1] * co1;
        __uint128_t t2   = (__uint128_t)cache[i-3] * co2;
        __uint128_t t3   = (__uint128_t)cache[i-2] * co3;
        __uint128_t num  = t1 + t2 - t3;
        __uint128_t den  = (2ULL * (i + 1ULL));
        return (seq_t)(num / den);
    } else {
        /* big‑integer seq_t: native operations are safe. */
        seq_t t1 = cache[i-1] * (seq_t)(2 * (5 * i - 1));
        seq_t t2 = cache[i-3] * (seq_t)(2 * (2 * i - 1));
        seq_t t3 = cache[i-2] * (seq_t)(3 * (3 * i - 1));
        seq_t num = t1 + t2 - t3;
        seq_t den = (seq_t)(2 * (i + 1));
        return num / den;
    }
#else
    /* For plain C: seq_t is an unsigned integer type; use 128‑bit intermediates. */
    __uint128_t co1 = (2ULL * (5ULL * i - 1ULL));
    __uint128_t co2 = (2ULL * (2ULL * i - 1ULL));
    __uint128_t co3 = (3ULL * (3ULL * i - 1ULL));
    __uint128_t t1   = (__uint128_t)cache[i-1] * co1;
    __uint128_t t2   = (__uint128_t)cache[i-3] * co2;
    __uint128_t t3   = (__uint128_t)cache[i-2] * co3;
    __uint128_t num  = t1 + t2 - t3;
    __uint128_t den  = (2ULL * (i + 1ULL));
    return (seq_t)(num / den);
#endif
}

/* Dynamic cache handling */
static seq_t *cache = NULL;      /* array of computed terms */
static idx_t cache_size = 0;     /* number of terms stored (next free index is cache_size) */
static size_t cache_cap = 0;     /* allocated capacity */

/* Ensure cache contains terms up to n; compute missing ones on demand. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;               /* already computed */

    /* Allocate or grow cache */
    if (cache_cap == 0) cache_cap = 4;
    while (cache_cap <= (size_t)n) cache_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failed */
    cache = tmp;

    /* Initialize base terms if this is the first call */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        cache_size = 4;
    }

    /* Compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* recurrence only applies for i >= 4; base values are already set */
        if (i >= 4) cache[i] = recurrence_term(i, cache);
    }
    cache_size = n + 1;
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
