/*
 * A201818  Numbers k such that 90*k + 49 is prime.
 * https://oeis.org/A201818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 296, u32 n <= 62, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer k for which 90*k + 49 is prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of found k values. */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len  = 0;
    static idx_t last_k_checked = 0;

    if (n == 0) return 0;   /* should never happen */

    /* Grow the cache until it contains at least n elements. */
    while ((idx_t)cache_len < n) {
        /* Look at the next candidate k. */
        last_k_checked++;
        seq_t candidate = (seq_t)90 * last_k_checked + (seq_t)49;

        if (is_prime_big(candidate)) {
            /* Ensure there is space in the cache. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
                if (!tmp) abort();      /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_k_checked;
        }
    }

    return cache[n - 1];
}
