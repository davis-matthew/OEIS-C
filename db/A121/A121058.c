/*
 * A121058  Positive integers x such that x+d+1 is composite for all divisors
 *          d of x.
 * https://oeis.org/A121058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 1000 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A121058: Positive integers x such that x + d + 1 is composite for all divisors d of x. */
/* The algorithm performs a brute‑force search with memoisation. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Utilities provided by the build system */
#include <stddef.h>
#include <limits.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *cache = NULL;         /* cache of found terms */
static size_t cache_len = 0;        /* number of terms cached */
static size_t cache_cap = 0;        /* current allocation capacity */
static seq_t next_check = 2;        /* smallest candidate not yet tested */

/* Test whether a single integer satisfies the definition. */
static bool is_candidate(seq_t x)
{
    /* limit = floor(sqrt(x)) */
    seq_t sqrt_x = floor_sqrt(x);
    idx_t limit = (idx_t)sqrt_x;   /* safe: limit is small for our use case */

    for (idx_t d = 1; d <= limit; ++d) {
        if ((x % (seq_t)d) == 0) {
            seq_t y1 = x + (seq_t)d + 1;
            if (isPrimeGeneric(y1))
                return false;

            seq_t d2 = x / (seq_t)d;
            if (d2 != (seq_t)d) {
                seq_t y2 = x + d2 + 1;
                if (isPrimeGeneric(y2))
                    return false;
            }
        }
    }
    return true;
}

/* Reallocate cache if needed. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
    if (!new_ptr) {
        /* In a production environment we would handle the allocation error.
           For the purposes of this sequence generator we simply abort. */
        abort();
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Main function that returns the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined, defensive guard */

    if (cache == NULL) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();
    }

    /* Find terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t cand = next_check++;
        if (is_candidate(cand)) {
            if (cache_len == cache_cap)
                grow_cache(cache_cap * 2);
            cache[cache_len++] = cand;
        }
    }
    return cache[n - 1];
}
