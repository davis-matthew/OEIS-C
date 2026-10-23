/*
 * A141891  Primes congruent to 8 mod 21.
 * https://oeis.org/A141891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p with p ≡ 8 (mod 21)
 * Range:     u8 n <= 5, u16 n <= 547, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;   /* number of primes stored */
    static idx_t last_checked = 2;

    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
    }

    while (cache_cnt < n) {
        if ((last_checked % 21) == 8 && is_prime(last_checked)) {
            if (cache_cnt == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_cnt++] = (seq_t)last_checked;
        }
        ++last_checked;
    }

    return cache[n - 1];
}
