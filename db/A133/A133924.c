/*
 * A133924  a(n) = number of exponents occurring only once each in the prime
 *          factorization of n!.
 * https://oeis.org/A133924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of exponents that occur exactly once in the *
 *            prime factorization of n! (OEIS A133924). This implementation
 *            uses * a sieve of Eratosthenes to generate all primes ≤ n and
 *            tallies the * exponents of each prime in n!; the result is the
 *            count of exponents * whose frequency is 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    size_t prime_cnt;
    idx_t *primes;
    build_primes(n, &primes, &prime_cnt);

    int *freq = (int *)calloc((size_t)n + 1, sizeof(int));
    if (!freq) { free(primes); return (seq_t)0; }

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p   = primes[i];
        idx_t pp  = p;
        idx_t exp = 0;
        while (pp <= n) {
            exp += n / pp;
            if (pp > n / p) break;
            pp *= p;
        }
        if (exp <= n)
            freq[(size_t)exp] += 1;
    }

    seq_t result = 0;
    for (size_t e = 1; e <= (size_t)n; ++e)
        if (freq[e] == 1)
            result += (seq_t)1;

    free(freq);
    free(primes);
    return result;
}
