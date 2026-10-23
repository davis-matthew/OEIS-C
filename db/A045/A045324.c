/*
 * A045324  Primes congruent to {0, 1, 2, 3, 4} (mod 7).
 * https://oeis.org/A045324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes congruent to 0,1,2,3,4 (mod 7) using on‑demand
   generation and lazy caching of the first n terms.  The implementation
   uses a simple trial‑division primality test suitable for the small
   values required (≤ ≈ 200 000 for n ≈ 10 000). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Cache for the sequence values.  The cached primes are stored in
   increasing order. */
static seq_t  *cache   = NULL;
static size_t  cache_cap = 0;
static idx_t   cache_size = 0;

/* The next integer to test for primality.  It is incremented after
   each check, so the loop always starts at the first number that
   hasn't been examined yet. */
static uint64_t next_candidate = 2;

static void ensure_cache(idx_t n)
{
    while (cache_size < n) {
        /* Find the next allowed prime. */
        while (true) {
            if (is_prime_u64(next_candidate) &&
                (next_candidate == 7 || (next_candidate % 7 <= 4)))
                break;
            ++next_candidate;
        }

        /* Grow the cache if necessary. */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the new term. */
        cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Offset is 1, but safety for n<1 */
    ensure_cache(n);
    return cache[n - 1];
}
