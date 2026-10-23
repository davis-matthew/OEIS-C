/*
 * A166655  Totally multiplicative sequence with a(p) = 6p-1 for prime p.
 * https://oeis.org/A166655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e in n of (6p-1)^e
 *            (Multiplicative, a(p) = 6p-1 for prime p) a(1) = 1 a(n) = Π (6*p
 *            - 1)^e (with p prime, e ≥ 1)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 1, and n=0 is out of range */
        return (seq_t)1;

    seq_t result = 1;
    idx_t temp = n;

    /* factor 2 separately */
    if ((temp & 1) == 0) {
        idx_t exp = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            exp++;
        }
        seq_t factor_val = 6 * (seq_t)2 - 1;   /* 11 */
        seq_t pow_val = 1;
        for (idx_t k = 0; k < exp; ++k)
            pow_val *= factor_val;
        result *= pow_val;
    }

    /* trial division by odd numbers */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            seq_t factor_val = 6 * (seq_t)p - 1;
            seq_t pow_val = 1;
            for (idx_t k = 0; k < exp; ++k)
                pow_val *= factor_val;
            result *= pow_val;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {               /* temp itself is prime */
        seq_t factor_val = 6 * (seq_t)temp - 1;
        result *= factor_val;    /* exponent = 1 */
    }

    return result;
}
