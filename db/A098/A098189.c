/*
 * A098189  Sum of unitary divisors minus Euler phi: a(n) = A034448(n) -
 *          A000010(n).
 * https://oeis.org/A098189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A034448(n) - A000010(n)
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* A034448(n) = product (1 + p_i^e_i) over prime factors p_i^e_i of n
       A000010(n) = φ(n)                           */
    if (n == 0) return 0;                      /* undefined for 0 */
    uint64_t temp = (uint64_t)n;

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(temp, primes, exps, &cnt);

    /* compute sum of unitary divisors */
    seq_t sigma_unit = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p_pow = (seq_t)1;
        for (uint64_t e = exps[i]; e > 0; --e) {
            p_pow *= (seq_t)primes[i];
        }
        sigma_unit *= ((seq_t)1 + p_pow);
    }

    /* compute Euler's totient */
    seq_t phi_val = phi(n);

    return sigma_unit - phi_val;
}
