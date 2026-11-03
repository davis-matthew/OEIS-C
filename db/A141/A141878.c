/*
 * A141878  Primes congruent to 13 mod 19.
 * https://oeis.org/A141878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime ≡ 13 (mod 19)
 * Range:     u8 n <= 4, u16 n <= 367, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 988 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Cached primes and state for incremental search. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 13;
static bool initialized = false;

seq_t a(idx_t n)
{
    /* One-time initialization. */
    if (!initialized) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        cache_len = 0;
        next_candidate = 13;
        initialized = true;
    }

    /* Generate primes until we have at least n terms. */
    while (cache_len < n) {
        if (is_prime(next_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 19;  /* next number ≡ 13 (mod 19) */
    }

    return cache[n - 1];
}
