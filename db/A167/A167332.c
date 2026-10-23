/*
 * A167332  Totally multiplicative sequence with a(p) = 2*(4*p-1) = 8*p-2 for
 *          prime p.
 * https://oeis.org/A167332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e||n} (8p − 2)^e (a totally multiplicative sequence)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 511, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1) = 1 */

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;             /* factor m = n */

    /* trial division to find prime factors */
    for (uint64_t p = 2; p <= m / p; p = (p == 2 ? 3 : p + 2)) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t base = (seq_t)(8 * p - 2);          /* 2*(4p-1) */
            for (uint64_t i = 0; i < exp; ++i) {
                result *= base;                       /* multiply base exp times */
            }
        }
    }

    /* If a prime factor > sqrt(n) remains */
    if (m > 1) {
        seq_t base = (seq_t)(8 * m - 2);              /* remaining prime factor */
        result *= base;
    }

    return result;
}
