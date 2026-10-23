/*
 * A142539  Primes congruent to 9 mod 53.
 * https://oeis.org/A142539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 9 (mod 53)
 * Range:     u8 none, u16 n <= 118, u32 n <= 119, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes and the last tested candidate. */
    static seq_t *primes = NULL;
    static idx_t count   = 0;
    static size_t capacity = 0;
    static seq_t candidate = 9;

    /* Return from cache if already computed. */
    if (n <= count) {
        return primes[n-1];
    }

    /* Grow the cache initially if necessary. */
    if (!primes) {
        capacity = 16;
        primes = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!primes) return 0; /* allocation failure */
    }

    /* Sieve candidates of the form 9 + 53*k until we find the n-th prime. */
    while (count < n) {
        candidate += (seq_t)53;                  /* next candidate */
        if (is_prime_big(candidate)) {           /* check primality */
            if (count == capacity) {             /* grow array */
                size_t new_cap = capacity * 2;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) return 0;             /* allocation failure */
                primes = tmp;
                capacity = new_cap;
            }
            primes[count++] = candidate;
        }
    }

    return primes[n-1];
}
