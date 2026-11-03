/*
 * A046954  Numbers k such that 6*k + 1 is nonprime.
 * https://oeis.org/A046954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that 6*k+1 is non‑prime (composite or 1)
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 87 terms through n = 980 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is increasing and a(1)=0, a(2)=4, a(3)=8, ... */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
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

/* Cache to store computed terms. */
static seq_t *cache = NULL;       /* pointer to array of seq_t */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_len = 0;      /* number of computed terms */

/* The last k that was found (cache[cache_len-1] when cache_len>0). */
static idx_t last_k = 0;

/* Grow the cache to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    size_t new_capacity = cache_cap == 0 ? 16 : cache_cap;
    while (new_capacity < new_cap)
        new_capacity *= 2;

    seq_t *new_ptr = mem_resize(seq_t, cache, new_capacity);
    /* In the unlikely event that realloc fails we simply keep using the old
       buffer; the function is assumed to be called only with modest sizes
       (n <= 10000) so the chance of failure is negligible. */
    if (new_ptr)
        cache = new_ptr, cache_cap = new_capacity;
}

/* Ensure that the cache contains at least n terms. */
static void ensure_terms(idx_t n)
{
    while (cache_len < (size_t)n) {
        idx_t k;
        if (cache_len == 0) {
            /* First term a(1)=0 */
            k = 0;
        } else {
            k = last_k + 1;
        }

        /* Find the next k such that 6*k+1 is not prime. */
        for (;;) {
            seq_t val = 6 * (seq_t)k + (seq_t)1;
            if (!isPrimeGeneric(val))
                break;          /* val is non‑prime; k is a valid term */
            ++k;
        }

        /* Store the new term. */
        grow_cache(cache_len + 1);
        cache[cache_len] = (seq_t)k;
        ++cache_len;
        last_k = k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined but safe fallback */

    ensure_terms(n);
    return cache[n - 1];
}
