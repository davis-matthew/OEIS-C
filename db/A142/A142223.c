/*
 * A142223  Primes congruent to 26 mod 41.
 * https://oeis.org/A142223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 146, u32 n <= 147, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: a(n) = n-th prime p with p ≡ 26 (mod 41) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* defines bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache     = NULL;
    static idx_t cap = 0;
    static idx_t count   = 0;
    static idx_t k_next  = 1;  /* next k so candidate = 26 + k*41 */

    /* initialise cache memory on first use */
    if (cache == NULL) {
        cap = 256;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* generate primes until we have at least n terms */
    while (count < n) {
        /* grow cache if necessary */
        if (count >= cap) {
            cap <<= 1;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }

        /* next candidate that is 26 mod 41 */
        seq_t candidate = (seq_t)26 + (seq_t)k_next * (seq_t)41;
        ++k_next;

        /* test primality */
        if (is_prime_big(candidate)) {
            cache[count++] = candidate;
        }
    }

    /* return the requested term (1‑based index) */
    return cache[n - 1];
}
