/*
 * A083346  Denominator of r(n) = Sum(e/p: n=Product(p^e)).
 * https://oeis.org/A083346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of all primes p dividing n such that p does not
 *            divide the exponent of p in the prime factorisation of n
 *            (equivalently, a(n) = n / gcd(n, n'), where n' is the arithmetic
 *            derivative of n)
 * Range:     u8 n <= 256, u16 n <= 9272, u32 n <= 9272, u64 n <= 9272,
 *            u128 n <= 9272, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    seq_t result = 1;

    /* factor by trial division up to sqrt(nn) */
    for (uint64_t p = 2ULL; (__int128)p * p <= (__int128)nn; ++p) {
        if (nn % p == 0ULL) {
            uint64_t e = 0ULL;
            while (nn % p == 0ULL) {
                nn /= p;
                ++e;
            }
            /* multiply if exponent e is NOT divisible by prime p */
            if (e % p != 0ULL) {
                result *= (seq_t)p;
            }
        }
    }

    /* if a prime factor larger than sqrt(original n) remains, its exponent is 1 */
    if (nn > 1ULL) {
        /* exponent e = 1, 1 % nn != 0 for nn > 1 */
        result *= (seq_t)nn;
    }

    return result;
}
