/*
 * A142659  Primes congruent to 43 mod 56.
 * https://oeis.org/A142659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p with p ≡ 43 (mod 56)
 * Range:     u8 n <= 2, u16 n <= 276, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Test primality of a 64‑bit unsigned integer.
   Works for all values that fit in seq_t for the range used. */
static int is_prime_uint64(uint64_t n) {
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return 0;
    }
    return 1;
}

/* Static cache of found primes. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;        /* number of primes stored */
static uint64_t next_candidate = 43; /* next number ≡ 43 (mod 56) to test */

/* Extend the cache until it contains at least n primes. */
static void extend_cache(idx_t n) {
    while (cache_size < n) {
        if (is_prime_uint64(next_candidate)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        next_candidate += 56;
    }
}

/* Return the nth prime congruent to 43 (mod 56). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined index; return 0 as safe guard */
    if (cache_size < n) extend_cache(n);
    return cache[n - 1];
}
