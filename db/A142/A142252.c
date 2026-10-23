/*
 * A142252  Primes congruent to 3 mod 43.
 * https://oeis.org/A142252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 148, u32 n <= 149, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A142252: Primes congruent to 3 mod 43
 * a(n) = nth prime p such that p mod 43 == 3
 * The function performs an efficient on‑demand search and stores
 * the primes found in a static cache.  The cache grows to a
 * sufficient size to accommodate the expected range (up to ≈10 000
 * terms).  The implementation uses the supplied is_prime_big
 * routine for primality testing, which works for all builds.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Maximum number of primes we expect to store (≈10 000 terms). */
#define PRIME_CAP 50000

static seq_t prime_cache[PRIME_CAP];
static size_t cache_used = 0;      /* number of primes currently cached */
static uint64_t next_candidate = 3;/* next number ≡3 (mod 43) to test */

/* Return the n-th prime p ≡ 3 (mod 43) (n ≥ 1).  Calls are thread‑unsafe. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 0; harmless */

    /* Convert n to an unsigned index for array access. */
    size_t target = (size_t)n;

    /* Ensure the cache contains at least 'target' elements. */
    while (cache_used < target) {
        if (cache_used == PRIME_CAP) {
            /* Should not happen for the intended range. */
            abort();
        }

        seq_t candidate = (seq_t)next_candidate;
        if (is_prime_big(candidate))
            prime_cache[cache_used++] = candidate;

        /* Advance to the next candidate congruent to 3 modulo 43. */
        next_candidate += 43;
    }

    /* Return the cached prime (array indexes are 0‑based). */
    return prime_cache[target - 1];
}
