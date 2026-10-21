/*
 * A058250  GCD of n-th primorial number and its totient.
 * https://oeis.org/A058250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = gcd(primorial(n), phi(primorial(n)))
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 26, u64 n <= 42, u128 n <= 72,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sieve_primes.h"

seq_t a(idx_t n) {
    /* n = index, a(0) = 1 (empty primorial) */
    if (n == 0) return (seq_t)1;

    /* obtain first n primes */
    idx_t limit = (n < 6) ? 15 : n * 20;  /* safe upper bound for n ≤ 10000 */
    idx_t *primes;
    size_t prime_cnt;
    sieve_primes(limit, &primes, &prime_cnt);
    while (prime_cnt < n) {   /* increase limit if necessary */
        limit *= 2;
        free(primes);
        sieve_primes(limit, &primes, &prime_cnt);
    }

    seq_t res = 1;
    for (idx_t i = 0; i < n; ++i) {
        idx_t pi = primes[i];
        bool found = false;
        for (idx_t j = i + 1; j < n; ++j) {
            if (primes[j] % pi == 1) {   /* pi divides (p_j-1) */
                found = true;
                break;
            }
        }
        if (found)
            res *= (seq_t)pi;
    }

    free(primes);
    return res;
}
