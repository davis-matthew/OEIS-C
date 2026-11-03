/*
 * A147594  a(n) = sigma_0(n + a(n-1)), with a(1) = 1.
 * https://oeis.org/A147594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 9059 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build: OEIS A147594
   Formula: a(n) = A000005(n + a(n-1)),  a(1) = 1
   Recurrence uses the divisor‑count function from divisor_count_u64.h.
   The implementation memoises values in a static array so that a(n)
   can be called in any order. */

#include <stdlib.h>   /* malloc, free */
#include <stdint.h>   /* uintptr_t, uint64_t */
#include <stddef.h>   /* size_t */
#include "../utils/mem.h"

/* Count divisors of a 64‑bit integer */
static uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t tau = 1;
    uint64_t exp;

    /* factor 2 */
    exp = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        ++exp;
    }
    if (exp) tau *= (exp + 1);

    /* factor odd primes */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }

    /* remaining prime factor */
    if (n > 1) tau *= 2;

    return tau;
}

static seq_t *cache = NULL;   /* 1‑based cache array */
static size_t cap = 0;        /* current capacity of cache */

/* Ensure the cache can hold indices up to 'needed' (inclusive). */
static void ensure_capacity(size_t needed)
{
    if (cap >= needed) return;

    size_t new_cap = (cap == 0 ? 2 : cap * 2);
    if (new_cap < needed) new_cap = needed;

    seq_t *new_cache = mem_new(seq_t, new_cap);
    if (!new_cache) return; /* allocation failed – leave cache unchanged */

    /* copy existing values */
    if (cache) {
        for (size_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];
        mem_free(cache);
    }

    cache = new_cache;
    cap   = new_cap;
}

/* Recursively compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    /* defined only for n >= 1 */
    if (n < 1)
        return (seq_t)0;

    /* ensure storage for this index */
    ensure_capacity((size_t)n + 1);

    /* base case */
    if (n == 1) {
        if (cache[1] == 0)
            cache[1] = (seq_t)1;
        return cache[1];
    }

    /* return cached value if available */
    if (cache[n] != 0)
        return cache[n];

    /* compute a(n-1) */
    seq_t prev = a(n - 1);

    /* compute divisor count of n + a(n-1) */
    uint64_t sum = (uint64_t)n + (uint64_t)prev;        /* fits in 64‑bit for this sequence */
    seq_t val = (seq_t)divisor_count_u64(sum);

    /* store and return */
    cache[n] = val;
    return val;
}
