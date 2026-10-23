/*
 * A045309  Primes congruent to {0, 2} mod 3.
 * https://oeis.org/A045309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p with p ≡ 0 or 2 (mod 3)
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t next_candidate = 2;

/* ensure capacity, realloc if necessary */
static void append_to_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* extend cache until it contains at least n terms */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        if (is_prime(next_candidate)) {
            if (next_candidate == 3 || (next_candidate % 3) == 2) {
                append_to_cache((seq_t)next_candidate);
            }
        }
        ++next_candidate;
    }
}

/* required public function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    ensure_cache(n);
    return cache[n - 1];
}
