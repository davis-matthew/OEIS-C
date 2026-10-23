/*
 * A162905  Primes of form p^2-6, p also a prime.
 * https://oeis.org/A162905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A137270(n))^2-6
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 17, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    static idx_t next_p = 3;

    if (cache_cnt == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        next_p = 3;
    }

    if (n > cache_cnt) {
        size_t required = n;
        if (required > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap < required) {
                new_cap *= 2;
            }
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        idx_t p = next_p;
        while (cache_cnt < n) {
            /* Find the next prime p. */
            while (!is_prime(p)) {
                ++p;
            }

            /* Compute p^2 - 6. */
            seq_t val = (seq_t)p * (seq_t)p - (seq_t)6;

            /* Check if the result is prime. */
            if (val > (seq_t)1 && is_prime_big(val)) {
                cache[cache_cnt++] = val;
            }
            ++p;
        }
        next_p = p;
    }

    return cache[n - 1];
}
