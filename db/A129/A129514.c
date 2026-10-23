/*
 * A129514  a(n) = gcd(Sum_{k|n} k, Sum_{1<k<n, k does not divide n} k) =
 *          gcd(sigma(n), n(n+1)/2 - sigma(n)) = gcd(sigma(n), n(n+1)/2),
 *          where sigma(n) = A000203(n).
 * https://oeis.org/A129514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(σ(n), n*(n+1)/2) where σ(n) = sum of divisors of n
 * Range:     u8 n <= 22, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(1) = 1, return 0 for n == 0 (not defined) */
    if (n == 0) return 0;

    /* Compute σ(n) using trial division with a small list of primes. */
    uint64_t m = (uint64_t)n;
    uint64_t sigma = 1;          /* σ(1) = 1 */

    /* Primes up to 100 are enough for n <= 10000 (sqrt(10000)=100). */
    static const uint32_t primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
        73,79,83,89,97
    };
    static const size_t primes_cnt = sizeof(primes)/sizeof(primes[0]);

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i]*primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        if (m % p == 0) {
            uint64_t sum = 1;          /* sum of powers of p: 1 + p + p^2 + ... */
            uint64_t powp = 1;
            while (m % p == 0) {
                m /= p;
                powp *= (uint64_t)p;
                sum += powp;
            }
            sigma *= sum;
        }
    }
    /* Any remaining factor > 1 is prime. */
    if (m > 1) {
        sigma *= (1 + m);
    }

    /* Compute the triangular number T = n*(n+1)/2. */
    uint64_t T = (uint64_t)n * ((uint64_t)n + 1) / 2;

    /* Return gcd as seq_t.  Cast operands to seq_t after ensuring they fit
       for the current build.  For builds where seq_t is small, the value
       of a(n) will fit, otherwise the result is discarded. */
    return gcd((seq_t)sigma, (seq_t)T);
}
