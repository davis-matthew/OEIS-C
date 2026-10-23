/*
 * A048785  a(0) = 0; a(n) = tau(n^3), where tau = number of divisors
 *          (A000005).
 * https://oeis.org/A048785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = tau(n³) (number of divisors of n³) a(0) = 0 (where tau(m)
 *            = product_i (exponent_i + 1) for m's prime factorization)
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    uint64_t val = (uint64_t)n;
    seq_t result = 1;
    uint64_t temp = val;

    /* Handle factor 2 separately for efficiency */
    if (temp % 2 == 0) {
        uint64_t exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++exp;
        }
        result *= (seq_t)(3 * exp + 1);
    }

    /* Remaining odd factors */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result *= (seq_t)(3 * exp + 1);
        }
    }

    /* If leftover is a prime > 1 */
    if (temp > 1) {
        result *= (seq_t)(3 * 1 + 1);  /* exponent = 1 */
    }

    return result;
}
