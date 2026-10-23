/*
 * A142368  Primes congruent to 17 mod 47.
 * https://oeis.org/A142368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 122, u32 n <= 123, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the primes congruent to 17 (mod 47) by incremental search
   and primality testing.  The first term is 17, each subsequent term
   is the next prime in the arithmetic progression 17 + 47·k. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of the primes found so far */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    if (n == 0) return 0;      /* offset is 1, values for n<1 are undefined */

    /* grow cache until we have at least n terms */
    while (len < n) {
        seq_t candidate;
        if (len == 0) {
            candidate = 17;          /* first member of the progression   */
        } else {
            candidate = cache[len - 1] + 47;
        }

        /* advance until we hit a prime */
        while (!is_prime_big(candidate)) {
            candidate += 47;
        }

        /* store the prime in the cache */
        if (len >= cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();     /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }
        cache[len++] = candidate;
    }

    return cache[n - 1];
}
