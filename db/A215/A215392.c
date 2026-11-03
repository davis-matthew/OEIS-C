/*
 * A215392  Primes congruent to {1, 2} mod 17.
 * https://oeis.org/A215392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p > 0 with p % 17 ∈ {1, 2}
 * Range:     u8 n <= 7, u16 n <= 824, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 983 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple incremental sieve with memoisation.
   It keeps a static cache of primes that satisfy the congruence
   condition.  On the first call it starts testing numbers from 2
   upwards, and only adds a number to the cache when it is prime and
   congruent to 1 or 2 modulo 17.  Subsequent calls reuse the cache
   without recomputation.  The cache grows as needed. */

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

/* Static cache of qualified primes */
static seq_t *prime_cache = NULL;
static size_t cache_count = 0;  /* number of primes stored */
static size_t cache_capacity = 0;  /* current allocated capacity */

/* The next candidate number to test */
static seq_t next_candidate = 2;

/* Return the n-th prime congruent to 1 or 2 modulo 17 */
seq_t a(idx_t n)
{
    /* Access to cache may need the target number of primes */
    size_t target = (size_t)n;   /* n fits in size_t because n is small */

    /* Ensure the cache contains at least 'target' primes */
    while (cache_count < target) {
        /* Test only if candidate ≡ 1 or 2 (mod 17) */
        seq_t rem = next_candidate % 17;
        if (rem == 1 || rem == 2) {
            /* If prime, store it */
            if (isPrimeGeneric(next_candidate)) {
                /* Grow cache if necessary */
                if (cache_count == cache_capacity) {
                    size_t new_cap = cache_capacity ? (cache_capacity << 1) : 8;
                    seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
                    if (!tmp) abort();   /* unrecoverable allocation failure */
                    prime_cache = tmp;
                    cache_capacity = new_cap;
                }
                prime_cache[cache_count++] = next_candidate;
            }
        }
        /* Advance to next integer */
        next_candidate++;
    }

    /* Return the requested prime (1‑based index) */
    return prime_cache[(size_t)(n - 1)];
}
