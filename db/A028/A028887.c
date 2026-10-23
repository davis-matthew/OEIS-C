/*
 * A028887  Theta series of 4-dimensional 5-modular lattice with det 25 and
 *          minimal norm 2.
 * https://oeis.org/A028887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * b(n), where b(n) is multiplicative: - b(0) = 1, -
 *            b(5^e) = 1, - for prime powers p^e (p≠5) : b(p^e) = (p^(e+1) –
 *            1)/(p – 1) (i.e. the divisor-sum σ, but with the factor for 5
 *            replaced by 1) This implementation factors n, skips the prime 5,
 *            and multiplies the arithmetic-geometric sum fo...
 * Range:     u8 n <= 23, u16 n <= 3527, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t result = 1;
    idx_t temp = n;

    /* factorization by trial division up to sqrt(temp) */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* count multiplicity */
            idx_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }

            if (p == 5U) {
                /* prime 5 contributes a factor of 1 – skip */
                continue;
            }

            /* compute factor = 1 + p + p^2 + ... + p^cnt */
            seq_t factor = 1;            /* for p^0 */
            seq_t term = 1;              /* p^i */
            for (idx_t i = 0; i < cnt; ++i) {
                term *= (seq_t)p;       /* next power p^{i+1} */
                factor += term;         /* sum up to this power */
            }
            result *= factor;           /* multiply into the running product */
        }
    }

    /* if remaining temp > 1 it's a prime factor > sqrt(original n) */
    if (temp > 1) {
        if (temp != 5U) {
            /* exponent is 1 -> factor = 1 + temp */
            result *= ((seq_t)1 + (seq_t)temp);
        }
        /* else temp == 5: ignore */
    }

    /* final multiplicative constant */
    result *= 6;
    return result;
}
