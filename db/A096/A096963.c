/*
 * A096963  a(n) = Sum {0<d|n, n/d odd} d^11.
 * https://oeis.org/A096963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum {0 < d | n, n/d odd} d^11
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 56, u128 n <= 3183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* not defined for 0, just return 0 */

    /* Count trailing zeros: exponent e of 2 in n */
    idx_t e = 0;
    idx_t temp = n;
    while ((temp & 1) == 0) {
        e++;
        temp >>= 1;
    }
    idx_t oddpart = temp;

    /* 2^(11*e) */
    seq_t result = 1;
    for (idx_t i = 0; i < e; i++) {
        result *= (seq_t)2048;   /* 2^11 = 2048 */
    }

    /* sum_{d|oddpart} d^11  (sigma_11 of oddpart) */
    seq_t sigma = 1;
    idx_t m = oddpart;
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                exp++;
                m /= p;
            }

            /* compute p^11 */
            seq_t p11 = 1;
            for (idx_t k = 0; k < 11; k++) {
                p11 *= (seq_t)p;
            }

            /* sum = 1 + p^11 + p^(22) + ... + p^(11*exp) */
            seq_t term_sum = 1;
            seq_t cur = 1;
            for (idx_t k = 1; k <= exp; k++) {
                cur *= p11;
                term_sum += cur;
            }

            sigma *= term_sum;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        idx_t p = m;
        idx_t exp = 1;   /* exponent is 1 because m is prime */

        seq_t p11 = 1;
        for (idx_t k = 0; k < 11; k++) {
            p11 *= (seq_t)p;
        }
        seq_t term_sum = 1 + p11;   /* 1 + p^11 */

        sigma *= term_sum;
    }

    result *= sigma;
    return result;
}
