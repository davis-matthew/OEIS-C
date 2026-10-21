/*
 * A067132  Number of elements in the largest set of divisors of n which are
 *          in geometric progression.
 * https://oeis.org/A067132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + max exponent among the prime factorisation of n (with
 *            a(1)=1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    uint64_t m = (uint64_t)n;
    uint64_t maxe = 0;

    /* factor out 2 */
    if ((m & 1ULL) == 0) {
        uint64_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            e++;
        }
        if (e > maxe) maxe = e;
    }

    /* trial division by odd numbers up to sqrt(m) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            if (e > maxe) maxe = e;
        }
    }

    /* if a prime factor > sqrt(original n) remains */
    if (m > 1) {
        /* its exponent is 1 */
        if (1 > maxe) maxe = 1;
    }

    return (seq_t)(maxe + 1);
}
