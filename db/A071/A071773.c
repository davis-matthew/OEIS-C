/*
 * A071773  a(n) = gcd(rad(n), n/rad(n)), where rad(n) = A007947(n) is the
 *          squarefree kernel of n.
 * https://oeis.org/A071773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of distinct primes p such that p^2 divides n (the
 *            squarefree kernel of the square part of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1.  For n ≥ 1 the result is always ≥ 1. */
    if (n <= 1) {
        return 1;
    }

    idx_t m = n;       /* element to be factored */
    seq_t result = 1;  /* value to be returned   */

    /* Trial division by all integers p up to sqrt(original n).
       The condition p <= m/p avoids overflow when p*p would exceed limits. */
    for (idx_t p = 2; p <= m / p; ++p) {
        idx_t exp = 0;
        while (m % p == 0) {
            ++exp;
            m /= p;
        }
        if (exp >= 2) {
            result = result * (seq_t)p;  /* multiply by the prime once */
        }
    }

    /* If m > 1 here, it is a prime factor with exponent 1,
       which does not contribute to a(n). */

    return result;
}
