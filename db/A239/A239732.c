/*
 * A239732  Primes p such that 2p+9 and 9p+2 are both prime.
 * https://oeis.org/A239732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 244, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 924 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds primes p such that 2p+9 and 9p+2 are also prime (OEIS A239732). */

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

/* Cached sequence storage */
static seq_t *cache       = NULL;
static size_t cache_len   = 0;      /* number of terms stored */
static size_t cache_cap   = 0;      /* allocated capacity */

/* Grow the cache array when needed */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 32;
        cache = mem_new(seq_t, cache_cap);
    } else {
        cache_cap *= 2;
        cache = mem_resize(seq_t, cache, cache_cap);
    }
}

/* Ensure that the cache holds at least 'target' terms */
static void ensure_cache(idx_t target)
{
    if (cache_len >= (size_t)target)
        return;   /* already enough */

    while (cache_len < (size_t)target) {
        seq_t p;
        if (cache_len == 0)                      /* first term */
            p = 5;
        else
            p = cache[cache_len - 1] + 2;        /* start after last found */

        /* Find the next qualifying prime */
        while (true) {
            /* Skip obvious composites */
            if (p > 5 && p % 5 == 0) {
                p += 2;
                continue;
            }
            if (!isPrimeGeneric(p)) {
                p += 2;
                continue;
            }

            seq_t two_p_plus_9  = p * 2 + 9;
            seq_t nine_p_plus_2 = p * 9 + 2;

            if (isPrimeGeneric(two_p_plus_9) && isPrimeGeneric(nine_p_plus_2)) {
                /* Store the term */
                if (cache_len == cache_cap)
                    grow_cache();

                cache[cache_len++] = p;
                break;   /* move to next term */
            }
            p += 2;  /* try next odd candidate */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1, but safe */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
