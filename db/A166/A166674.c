/*
 * A166674  Totally multiplicative sequence with a(p) = 5p+2 for prime p.
 * https://oeis.org/A166674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e||n} (5p+2)^e (product over all prime powers of n)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* Loop over possible prime divisors up to sqrt(temp) */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t base = (seq_t)(5 * p + 2);
            seq_t pow = (seq_t)1;
            for (idx_t i = 0; i < exp; ++i)
                pow *= base;
            result *= pow;
        }
    }

    /* If there's a remaining prime factor greater than sqrt(n) */
    if (temp > 1) {
        seq_t base = (seq_t)(5 * temp + 2);
        result *= base;
    }

    return result;
}
