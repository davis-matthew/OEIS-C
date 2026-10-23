/*
 * A136176  Number of exponents in the prime factorization of n which divide
 *          n.
 * https://oeis.org/A136176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of exponents e in prime factorisation of n * such
 *            that n mod e = 0 * * The function enumerates the prime
 *            factorisation of n, checks each * exponent e, and counts how
 *            many of them divide n. It works for all * n >= 1 and has a time
 *            complexity of O(sqrt(n)), which is well * under 2 s f...
 * Range:     u8 n <= 9692, u16 n <= 9692, u32 n <= 9692, u64 n <= 9692,
 *            u128 n <= 9692, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;            /* n = 1 has no prime factors */

    seq_t count = 0;
    uint64_t temp = n;

    /* trial division by 2 first */
    if (temp % 2 == 0) {
        int exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            exp++;
        }
        if (n % (uint64_t)exp == 0)
            count++;
    }

    /* odd primes */
    uint64_t p = 3;
    while (p * p <= temp) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if (n % (uint64_t)exp == 0)
                count++;
        }
        p += 2;
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        /* exponent is 1, and 1 divides every integer */
        count++;   /* exponent e = 1 always satisfies n mod 1 == 0 */
    }

    return count;
}
