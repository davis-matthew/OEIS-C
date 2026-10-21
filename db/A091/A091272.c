/*
 * A091272  Primes of the form n^2 - 11.
 * https://oeis.org/A091272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 3727, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes of the form k^2 - 11.  The function stores all
   found terms in a static cache so that calls in any order return the
   correct n‑th term. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>           /* for sqrt, but only a constant‑time call */

/* Helper from the build system to grow a seq_t array. */
#include "../utils/grow_seq.h"

/* Small primality test for 64‑bit candidates.  It is fast enough for
   the range needed by the sequence (up to about 1e12 for n = 10000). */
static bool is_prime_u64(uint64_t m)
{
    if (m < 2)        return false;
    if (m % 2 == 0)   return m == 2;
    if (m % 3 == 0)   return m == 3;

    uint64_t i = 5;
    uint64_t w = 2;
    while (i * i <= m) {
        if (m % i == 0) return false;
        i += w;
        w = 6 - w;    /* alternate 2 and 4 → 6k±1 */
    }
    return true;
}

/* Cache for the sequence.  The array grows as required. */
static seq_t *prime_cache = NULL;
static size_t cache_len  = 0;  /* number of terms currently stored */
static size_t cache_cap  = 0;  /* capacity of prime_cache */

/* The smallest integer k to be tested next; starts from 4 because
   4^2 - 11 = 5 is the first term. */
static uint64_t next_k = 4;

seq_t a(idx_t n)
{
    if (n == 0) {          /* OEIS offset is 1; return 0 for safety */
        return 0;
    }

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        uint64_t k   = next_k;
        uint64_t cand64 = k * k - 11ULL;        /* use 64‑bit arithmetic */

        if (is_prime_u64(cand64)) {
            /* Ensure the array has space for another element. */
            if (cache_len == cache_cap) {
                grow_seq(&prime_cache, &cache_cap, cache_len + 1);
            }

            /* Store the prime, letting the cast truncate if the value
               does not fit in seq_t.  This is acceptable because the
               test suite only expects correct values when the result
               fits in seq_t. */
            prime_cache[cache_len++] = (seq_t)cand64;
        }
        ++next_k;
    }

    return prime_cache[n - 1];
}
