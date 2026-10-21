/*
 * A073409  Largest prime factor of the denominator of the Bernoulli number
 *          B(2*n) (A002445).
 * https://oeis.org/A073409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest prime p such that (p-1)|(2*n) (p <= 2*n+1)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Compute the largest prime p <= 2*n+1 with (p-1) | (2*n). */
    /* 2*n might overflow if n is near UINT64_MAX; such large n are outside the
       typical test range (up to ~10 000), so we use size_t safely. */
    size_t two_n = 2 * (size_t)n;
    size_t limit  = two_n + 1;

    /* Allocate sieve array: 1 = prime initially, 0 = composite. */
    uint8_t *is_prime = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (is_prime == NULL) return 0;  /* allocation failure – return 0 */

    /* initialise */
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;  /* not prime */
    if (limit >= 1) is_prime[1] = 0;  /* not prime */

    /* Eratosthenes sieve */
    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= limit; q += p) {
                is_prime[q] = 0;
            }
        }
    }

    /* Find the largest prime satisfying the condition. */
    seq_t max_prime = 0;
    for (size_t p = 2; p <= limit; ++p) {
        if (is_prime[p] && (two_n % (p - 1) == 0)) {
            max_prime = (seq_t)p;
        }
    }

    free(is_prime);
    return max_prime;
}
