/*
 * A138700  Numbers k such that 2*k+1, 3*k+2, 4*k+3 and 5*k+4 are primes.
 * https://oeis.org/A138700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 17, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search method: iterate k ≡ 5 (mod 6) and test primality of
 * 2*k+1, 3*k+2, 4*k+3, and 5*k+4. */
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Grow cache until it holds at least n terms. */
    while (cache_len < (size_t)n) {
        /* Current candidate k.  Start at 5 or continue from the
         * most recent term with the same congruence (k ≡ 5 mod 6). */
        seq_t k = (cache_len == 0) ? 5 : cache[cache_len - 1] + 6;

        /* Find next k satisfying the prime conditions. */
        for (;;) {
            if (is_prime_big(k * 2 + 1) &&
                is_prime_big(k * 3 + 2) &&
                is_prime_big(k * 4 + 3) &&
                is_prime_big(k * 5 + 4)) {

                /* Ensure space in cache. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                break;      /* proceed to next term */
            }
            k += 6;          /* preserve k ≡ 5 (mod 6) */
        }
    }

    return cache[n - 1];   /* a(1) is the first element */
}
