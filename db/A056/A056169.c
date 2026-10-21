/*
 * A056169  Number of unitary prime divisors of n.
 * https://oeis.org/A056169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = number of unitary prime divisors of n (primes p such that
 *            p divides n and p^2 does not divide n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                /* a(1) = 0 */

    uint64_t x = (uint64_t)n;     /* work with 64‑bit integer */
    seq_t count = 0;              /* result */
    uint64_t temp = x;

    /* factor 2 separately for speed */
    if ((temp & 1ULL) == 0) {
        uint8_t exp = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            exp++;
        }
        if (exp == 1)
            count++;
    }

    /* trial division over odd candidates */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint8_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if (exp == 1)
                count++;
        }
    }

    /* if anything remains it is a prime factor with exponent 1 */
    if (temp > 1)
        count++;

    return count;
}
