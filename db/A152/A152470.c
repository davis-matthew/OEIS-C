/*
 * A152470  Largest of three consecutive primes whose sum is a prime.
 * https://oeis.org/A152470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest of three consecutive primes whose sum is prime.
 *            The function searches for the nth such triple by generating
 *            primes until enough successes are found
 * Range:     u8 n <= 24, u16 n <= 1658, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_primes.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but guard against underflow */

    idx_t limit = n * 200;               /* initial estimate */
    if (limit < 1000) limit = 1000;      /* minimal bound */

    seq_t result = 0;

    for (;;) {
        uint64_t *primes = NULL;
        size_t prime_cnt = 0;
        /* build primes up to current limit */
        sieve_primes(limit, (idx_t **) &primes, &prime_cnt);

        size_t found = 0;
        for (size_t i = 0; i + 2 < prime_cnt; ++i) {
            uint64_t sum = primes[i] + primes[i + 1] + primes[i + 2];
            if (is_prime((idx_t)sum)) {
                ++found;
                if (found == n) {
                    result = (seq_t)primes[i + 2];
                    free(primes);
                    return result;
                }
            }
        }

        /* not enough successes, increase the range and try again */
        free(primes);
        limit *= 2;
    }
}
