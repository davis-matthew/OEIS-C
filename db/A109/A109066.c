/*
 * A109066  Number of prime digits in n-th prime.
 * https://oeis.org/A109066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = count of prime digits (2,3,5,7) in the n‑th prime (OEIS
 *            A109066). The implementation generates primes on demand using a
 *            simple sieve up to an upper bound provided by
 *            estimate_prime_upper_bound
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/estimate_prime_upper_bound.h"

extern seq_t a(idx_t n)
{
    /* nothing needed for n < 1 */
    if (n == 0) return (seq_t)0;

    /* static data that persists across calls */
    static uint64_t *primes = NULL;   /* array of found primes */
    static size_t prime_cnt = 0;      /* number of primes stored          */
    static size_t prime_cap = 0;      /* capacity of the primes array     */
    static uint8_t  *sieve  = NULL;   /* bitmask for composite numbers    */
    static size_t sieve_limit = 0;    /* last sieve size                  */

    /* if we don't have enough primes, extend the sieve */
    if (prime_cnt < (size_t)n) {
        /* compute an upper bound on the n‑th prime */
        idx_t  ub  = estimate_prime_upper_bound(n);
        size_t limit = (size_t)ub;

        /* free old sieve array */
        free(sieve);
        sieve = (uint8_t*)calloc(limit + 1, 1);
        if (sieve == NULL) return (seq_t)0;   /* out of memory */

        /* classic Eratosthenes sieve */
        for (size_t i = 2; i * i <= limit; ++i) {
            if (!sieve[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    sieve[j] = 1;
            }
        }

        /* collect primes */
        size_t new_cnt = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (!sieve[i]) ++new_cnt;

        /* reallocate primes array to the new size */
        uint64_t *new_primes = (uint64_t*)realloc(primes, new_cnt * sizeof(uint64_t));
        if (new_primes == NULL) {
            free(sieve);
            return (seq_t)0;   /* out of memory */
        }
        primes = new_primes;
        /* store primes into the array */
        size_t idx = 0;
        for (size_t i = 2; i <= limit; ++i) {
            if (!sieve[i]) {
                primes[idx++] = (uint64_t)i;
            }
        }
        prime_cnt = new_cnt;
        prime_cap = new_cnt;
        sieve_limit = limit;
    }

    /* retrieve the n‑th prime (1‑based index) */
    uint64_t p = primes[n - 1];

    /* count digits that are prime digits 2,3,5,7 */
    seq_t count = 0;
    while (p > 0) {
        unsigned d = (unsigned)(p % 10);
        if (d == 2 || d == 3 || d == 5 || d == 7)
            ++count;
        p /= 10;
    }

    return count;
}
