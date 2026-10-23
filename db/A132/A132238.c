/*
 * A132238  Primes congruent to {11, 13} mod 30.
 * https://oeis.org/A132238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes n‑th prime congruent to 11 or 13 modulo 30, offset 1 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset 1; behaviour below that does not matter */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 11; /* first prime in the sequence */

    while (cache_size < n) {
        /* Test whether the current candidate is prime and has the required residue */
        if (is_prime(next_candidate) &&
            ((next_candidate % 30ULL) == 11ULL || (next_candidate % 30ULL) == 13ULL)) {

            /* Grow the cache if necessary */
            if (cache_size + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
