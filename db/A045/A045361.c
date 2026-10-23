/*
 * A045361  Primes congruent to {2, 4, 5, 6} mod 7.
 * https://oeis.org/A045361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045361: primes congruent to 2,4,5 or 6 mod 7
   a(n) = the nth prime p such that p % 7 ∈ {2,4,5,6}  (n ≥ 1)  */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* static storage for cached primes */
static seq_t *prime_cache = NULL;   /* array of found primes  */
static size_t cache_size = 0;       /* number of primes stored */
static size_t cache_cap  = 0;       /* capacity of prime_cache  */
static uint64_t next_candidate = 2; /* next number to test */

/* test whether a 64‑bit number has a residue allowed by the sequence */
static inline bool good_residue(uint64_t v) {
    uint64_t r = v % 7;
    return r == 2 || r == 4 || r == 5 || r == 6;
}

/* enlarge the cache to at least 'needed' elements */
static void grow_cache(size_t needed) {
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)seq_realloc(prime_cache, cache_cap, new_cap);
    prime_cache = new_cache;
    cache_cap  = new_cap;
}

/* sequence generator */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;               /* undefined for n < 1, but any value works */

    while (cache_size < (size_t)n) {
        /* skip numbers that cannot be in the sequence */
        while (!good_residue(next_candidate))
            ++next_candidate;

        /* test the current candidate for primality */
        if (is_prime_big((seq_t)next_candidate)) {
            grow_cache(cache_size + 1);
            prime_cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return prime_cache[n - 1];
}
