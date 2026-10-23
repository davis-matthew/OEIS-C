/*
 * A142435  Primes congruent to 25 mod 49.
 * https://oeis.org/A142435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 136, u32 n <= 137, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n-th prime congruent to 25 mod 49
 * (OEIS A142435). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                         /* sequence is defined for n ≥ 1 */
    }

    /* Static cache of already-found terms */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t next_k  = 0;          /* next k such that 49*k+25 is checked */

    while (cache_len < n) {
        bool found = false;
        while (!found) {
            /* candidate = 49 * next_k + 25  (all arithmetic in seq_t) */
            seq_t candidate = (seq_t)49ULL * (seq_t)next_k + (seq_t)25ULL;

            if (is_prime_big(candidate)) {
                /* Grow cache if needed */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* allocation failed – return zero (error) */
                        return 0;
                    }
                    cache     = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = candidate;
                found = true;
            }
            ++next_k;
        }
    }
    return cache[n - 1];
}
