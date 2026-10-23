/*
 * A142381  Primes congruent to 30 mod 47.
 * https://oeis.org/A142381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 117, u32 n <= 118, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: find primes congruent to 30 (mod 47) by linear search. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 30;   /* starts at 30, the first number ≡30 (mod 47) */

static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* n must be ≥ 1 */
    while (cache_len < n) {
        /* Advance candidate until a prime is found */
        while (!(next_candidate >= 2 && is_prime_big(next_candidate))) {
            next_candidate += 47;
        }
        /* Store found prime */
        ensure_cache_cap(cache_len + 1);
        cache[cache_len++] = next_candidate;
        next_candidate += 47;  /* next candidate ≡30 (mod 47) */
    }
    return cache[n - 1];
}
