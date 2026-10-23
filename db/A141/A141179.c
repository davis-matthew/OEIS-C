/*
 * A141179  Primes of the form 3*x^2 + 2*x*y - 3*y^2 (as well as of the form
 *          3*x^2 + 8*x*y + 2*y^2).
 * https://oeis.org/A141179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes of the form 3*x^2 + 2*x*y - 3*y^2 or 3*x^2 + 8*x*y
 *            + 2*y^2 Implemented by generating primes and selecting those
 *            with (p % 240)^2 ≡ 169 (mod 240) (plus the trivial primes 2, 3,
 *            5)
 * Range:     u8 n <= 15, u16 n <= 1638, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 7790, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* cached representable primes */
    static size_t cache_len = 0;         /* number of cached terms */
    static size_t cache_cap = 0;         /* capacity of cache array */
    static idx_t last_checked = 1;       /* last integer examined for primality */

    /* Allocate or grow cache as needed */
    if (cache_len < n) {
        /* Ensure sufficient capacity for the cache */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(*cache));
            if (!tmp) { /* allocation failed; abort gracefully */
                abort();
            }
            cache = tmp;
        }

        /* Find representable primes until we have enough terms */
        for (;;) {
            /* Search for the next prime */
            do {
                last_checked++;
            } while (!is_prime(last_checked));

            if (last_checked < 6) {                 /* 2, 3, 5 are always included */
                cache[cache_len++] = (seq_t)last_checked;
            } else {
                idx_t rem = last_checked % 240;
                if ((rem * rem) % 240 == 169) {     /* quadratic residue test */
                    cache[cache_len++] = (seq_t)last_checked;
                }
            }

            if (cache_len >= n) break;
        }
    }

    return cache[n - 1];
}
