/*
 * A142315  Primes congruent to 7 mod 45.
 * https://oeis.org/A142315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 274, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate primes congruent to 7 (mod 90) */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_realloc.h"

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of entries currently stored   */
static size_t cache_cap = 0;   /* allocated capacity                   */
static seq_t next_candidate = (seq_t)7;   /* next number to test, 7 mod 90 */

seq_t a(idx_t n)
{
    if (n == 0)          /* out‑of‑range, but never requested */
        return (seq_t)0;

    /* Grow cache if necessary */
    if (cache_len < (size_t)n) {
        size_t required = (size_t)n;
        if (required > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < required) new_cap *= 2;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        /* Generate primes until we have n entries in the cache */
        while (cache_len < required) {
            if (isPrimeGeneric(next_candidate)) {
                cache[cache_len++] = next_candidate;
            }
            next_candidate += (seq_t)90;
        }
    }

    return cache[n - 1];
}
