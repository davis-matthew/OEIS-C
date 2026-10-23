/*
 * A129916  Numbers k such that adding the product of the digits of k to the
 *          product of k and the sum of its digits yields a prime.
 * https://oeis.org/A129916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) are the integers k such that k*digit_sum(k) +
 *            digit_product(k) is prime, listed in increasing order
 * Range:     u8 n <= 3, u16 n <= 92, u32 n <= 93, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, digit_sum, ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* cache of found terms, built lazily and kept between calls */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next candidate to test; starts at 1 */
    static seq_t candidate = 1;

    if (n == 0)
        return 0;                 /* undefined for offset 0 */

    while (cache_len < n) {
        /* compute digit sum and product of the current candidate */
        seq_t ds = digit_sum(candidate);
        seq_t dp = digit_product(candidate);
        seq_t test = candidate * ds + dp;

        /* if the test value is prime, store the candidate */
        if (is_prime_big(test)) {
            ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache,
                            sizeof(seq_t));
            cache[cache_len++] = candidate;
        }
        /* move to the next integer to test */
        candidate++;
    }

    return cache[n - 1];
}
