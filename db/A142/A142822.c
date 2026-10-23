/*
 * A142822  Primes congruent to 24 mod 61.
 * https://oeis.org/A142822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 24 (mod 61) by incremental search
   (OEIS A142822). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 17;        /* 61*17 + 24 = 1061 */

    while (cache_size < n) {
        idx_t cand = 61 * next_k + 24;   /* next candidate of form 61*k+24 */

        /* Test cand for primality */
        if (is_prime(cand)) {
            if (cache_size + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)cand;   /* store result */
        }

        ++next_k;
    }

    return cache[n - 1];
}
