/*
 * A173557  a(n) = Product_{primes p dividing n} (p-1).
 * https://oeis.org/A173557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Product_{p|n, p prime} (p-1) (distinct primes only)
 * Range:     u8 n <= 256, u16 n <= 9616, u32 n <= 9616, u64 n <= 9616,
 *            u128 n <= 9616, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes OEIS A173557
 * a(1) = 1, for n > 1 a(n) = Π_{p|n} (p-1). */
seq_t a(idx_t n)
{
    /* Handle the trivial case. */
    if (n <= 1)
        return (seq_t)1;

    seq_t      result = 1;
    uint64_t   m      = (uint64_t)n;

    /* trial division up to sqrt(m) */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if ((m % p) == 0) {
            /* p is a prime divisor */
            result *= (seq_t)(p - 1);
            /* strip all powers of p */
            do {
                m /= p;
            } while ((m % p) == 0);
        }
    }

    /* if remainder is > 1, it is a prime > sqrt(original n) */
    if (m > 1) {
        result *= (seq_t)(m - 1);
    }

    return result;
}
