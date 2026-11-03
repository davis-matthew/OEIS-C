/*
 * A142405  Primes congruent to 23 mod 48.
 * https://oeis.org/A142405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 23 (mod 48)
 * Range:     u8 n <= 3, u16 n <= 414, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 925 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search candidates ≡ 23 (mod 48) and test primality using is_prime */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

static seq_t *prime_cache = NULL;
static size_t   cache_len = 0;
static size_t   cache_cap = 0;
static idx_t    next_candidate = 23;

seq_t a(idx_t n)
{
    if (!prime_cache) {
        cache_cap = 16;
        prime_cache = mem_new(seq_t, cache_cap);
    }

    while (cache_len < n) {
        if (is_prime(next_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                prime_cache = mem_resize(seq_t, prime_cache, cache_cap);
            }
            prime_cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
        }
        next_candidate += 48;
    }

    return prime_cache[n - 1];
}
