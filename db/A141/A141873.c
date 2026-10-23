/*
 * A141873  Primes congruent to 8 mod 19.
 * https://oeis.org/A141873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 8 (mod 19)
 * Range:     u8 n <= 2, u16 n <= 358, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterate through integers, test primality with is_prime(),
   collect those ≡ 8 (mod 19), and cache results so that subsequent
   calls are fast regardless of order. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *prime_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_prime = 0;

static void ensure_cache_capacity(size_t needed) {
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    prime_cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1; return 0 for safety */

    if (cache_len >= n) return prime_cache[n - 1];

    for (size_t target = cache_len + 1; target <= n; ++target) {
        uint64_t candidate = last_prime + 1;
        if (candidate < 2) candidate = 2;

        for (;;) {
            if ((candidate % 19ULL) == 8ULL && is_prime((idx_t)candidate)) {
                ensure_cache_capacity(cache_len + 1);
                prime_cache[cache_len++] = (seq_t)candidate;
                last_prime = candidate;
                break;
            }
            ++candidate;
        }
    }
    return prime_cache[n - 1];
}
