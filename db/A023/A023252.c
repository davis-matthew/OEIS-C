/*
 * A023252  Primes that remain prime through 2 iterations of function f(x) =
 *          5x + 2.
 * https://oeis.org/A023252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that 5p+2 and 25p+12 are also
 *            primes
 * Range:     u8 none, u16 n <= 27, u32 n <= 32, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* A static cache that holds all terms already found. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* The next candidate prime to test. */
    static seq_t cur = 2;

    /* Initialise the cache the first time the function is called. */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    }

    /* Generate additional terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Find the next prime candidate. */
        do {
            cur++;
            /* Skip even numbers after the first prime 2. */
            if ((cur & 1) == 0)
                continue;
        } while (!is_prime_big(cur));

        seq_t p = cur;
        seq_t q = p * (seq_t)5 + (seq_t)2;      /* 5p + 2 */
        seq_t r = q * (seq_t)5 + (seq_t)2;      /* 25p + 12 */

        /* If both derived numbers are prime, store p. */
        if (is_prime_big(q) && is_prime_big(r)) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = p;
        }
    }

    return cache[n - 1];
}
