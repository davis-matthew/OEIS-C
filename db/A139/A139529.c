/*
 * A139529  Numbers k such that 24*k + 13 is prime.
 * https://oeis.org/A139529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k where 24*k + 13 is prime, with offset 1
 * Range:     u8 n <= 7, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
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

static seq_t *cache = NULL;      /* cached k values (a(1), a(2), …) */
static idx_t cache_size = 0;     /* number of cached terms */
static idx_t cache_cap   = 0;     /* capacity of the cache array */

/* Double the cache capacity when needed. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();                    /* allocation failed */
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/* Return the nth term of OEIS A139529 (k such that 24*k+13 is prime). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 – guard */

    /* Ensure the cache contains at least n terms. */
    if (cache_size < n) {
        /* Start from the next integer after the last cached k. */
        idx_t k = cache_size ? (idx_t)cache[cache_size - 1] + 1 : 0;

        while (cache_size < n) {
            if (cache_cap <= cache_size) grow_cache();

            /* Candidate prime: 24*k + 13 */
            seq_t candidate = (seq_t)(24 * k + 13);

            if (isPrimeGeneric(candidate))
                cache[cache_size++] = (seq_t)k;

            ++k;
        }
    }

    return cache[n - 1];
}
