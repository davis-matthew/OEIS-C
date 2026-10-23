/*
 * A142318  Primes congruent to 13 mod 45.
 * https://oeis.org/A142318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 279, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes congruent to 13 modulo 45 (OEIS A142318) */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n) {
    if (n <= 3) return n > 1;
    if ((n & 1U) == 0) return false;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1 – behaviour below is irrelevant */

    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t sz   = 0;           /* number of primes stored */
    static size_t cap  = 0;           /* current capacity of cache */
    static uint64_t next_candidate = 13; /* first prime in the progression */

    /* grow cache when needed */
    if (sz == 0) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    while (sz < n) {
        if (is_prime_uint64(next_candidate)) {
            /* store the prime, cast to seq_t (overflow is fine when value does not fit) */
            if (sz == cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[sz++] = (seq_t)next_candidate;
        }
        next_candidate += 45;
    }

    return cache[n - 1];
}
