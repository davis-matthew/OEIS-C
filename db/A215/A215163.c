/*
 * A215163  Primes congruent to {1, 4} mod 11.
 * https://oeis.org/A215163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 964, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 1 or 4 (mod 11) */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 1;

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    if (cache_len >= (size_t)n) {
        return cache[n - 1];
    }

    for (seq_t candidate = last_checked + 1; ; ++candidate) {
        if (is_prime_big(candidate)) {
            unsigned int r = (unsigned int)(candidate % 11);
            if (r == 1U || r == 4U) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        return 0;
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = candidate;
                if (cache_len == (size_t)n) {
                    last_checked = candidate;
                    return candidate;
                }
            }
        }
        last_checked = candidate;
    }
}
