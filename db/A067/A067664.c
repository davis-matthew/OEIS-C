/*
 * A067664  Numbers n such that n^2 + 1 and n^2 + n + 1 are primes.
 * https://oeis.org/A067664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth integer m such that m^2+1 and m^2+m+1 are primes
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 553, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 65 terms through n = 4046 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void extend_cache(idx_t target)
{
    while ((idx_t)cache_len < target)
    {
        /* Start searching from the next integer */
        seq_t last = (cache_len == 0) ? (seq_t)0 : cache[cache_len - 1];
        seq_t m = last + (seq_t)1;

        for (;;)
        {
            seq_t mm  = m * m;
            seq_t a   = mm + (seq_t)1;
            seq_t b   = mm + m + (seq_t)1;

            if (isPrimeGeneric(a) && isPrimeGeneric(b))
            {
                /* Ensure capacity for the new element */
                if (cache_cap == cache_len)
                {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *newp    = seq_realloc(cache, cache_cap, new_cap);
                    cache          = newp;
                    cache_cap      = new_cap;
                }
                cache[cache_len++] = m;
                break; /* candidate found, move to next n */
            }
            m = m + (seq_t)1;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 – behaviour for 0 is undefined */
    extend_cache(n);
    return cache[n - 1];
}
