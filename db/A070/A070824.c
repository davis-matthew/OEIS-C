/*
 * A070824  Number of divisors of n which are > 1 and < n (nontrivial
 *          divisors).
 * https://oeis.org/A070824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = τ(n) – 2, where τ(n) is the number of divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1u)
        return 0;

    uint64_t m = (uint64_t)n;
    uint64_t d = 1;

    /* trial division up to sqrt(m) */
    uint64_t p = 2;
    while (p <= m / p) {
        if (m % p == 0) {
            unsigned int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            d *= (uint64_t)(exp + 1);
        }
        ++p;
    }

    /* remaining prime factor, if any */
    if (m > 1)
        d *= 2;

    return (seq_t)(d - 2);
}
