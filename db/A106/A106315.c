/*
 * A106315  Harmonic residue of n.
 * https://oeis.org/A106315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n * d(n)) mod sigma(n)
 * Range:     u8 n <= 16, u16 n <= 4669, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(n) is the number of divisors of n and sigma(n) is the sum of divisors of n. */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    idx_t m = n;
    seq_t tau = 1;   /* number of divisors d(n) */
    seq_t sigma = 1; /* sum of divisors sigma(n) */

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            tau *= (seq_t)(e + 1);

            /* sigma part for prime power p^e: (p^(e+1)-1)/(p-1) */
            seq_t pow_p = 1;
            for (unsigned i = 0; i < e + 1; ++i)
                pow_p *= (seq_t)p;
            seq_t sigma_part = (pow_p - 1) / (seq_t)(p - 1);
            sigma *= sigma_part;
        }
    }

    if (m > 1) {   /* remaining prime factor */
        tau *= 2;
        seq_t pow_p = (seq_t)m * (seq_t)m; /* p^2 */
        seq_t sigma_part = (pow_p - 1) / (seq_t)(m - 1);
        sigma *= sigma_part;
    }

    seq_t product = nn * tau;
    return product % sigma;
}
