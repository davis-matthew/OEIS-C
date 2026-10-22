/*
 * A006955  Denominator of (2n+1) B_{2n}, where B_n are the Bernoulli numbers.
 * https://oeis.org/A006955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   a(0) = 1; for n >= 1, a(n) = Product_{p prime, (p-1) | 2n, p
 *            does not divide 2n+1} p (von Staudt-Clausen: denominator(B_{2n})
 *            = Product_{p prime, (p-1) | 2n} p, which is squarefree and
 *            coprime to the numerator, so multiplying by 2n+1 cancels exactly
 *            the primes dividing 2n+1)
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 89, u64 n <= 419,
 *            u128 n <= 1000, big any n
 * Helpers:   a006955_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a006955_isprime.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    uint64_t m = 2 * (uint64_t)n;
    uint64_t odd = m + 1;
    seq_t r = 1;
    for (uint64_t d = 1; d * d <= m; d++) {
        if (m % d != 0) continue;
        uint64_t e = m / d;
        uint64_t p = d + 1;
        if (a006955_isprime(p) && odd % p != 0) r *= (seq_t)p;
        if (e != d) {
            p = e + 1;
            if (a006955_isprime(p) && odd % p != 0) r *= (seq_t)p;
        }
    }
    return r;
}
