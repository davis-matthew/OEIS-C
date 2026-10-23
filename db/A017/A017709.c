/*
 * A017709  Numerator of sum of -23rd powers of divisors of n.
 * https://oeis.org/A017709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (∑_{d|n} 1/d^23) in lowest terms, i.e. a(n)
 *            = σ_{23}(n) / gcd(σ_{23}(n), n^23)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 47,
 *            big any n
 * Helpers:   factor_u64, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"      /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/seq_gcd.h"         /* seq_gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1. */
    if (n == 1)
        return (seq_t)1;

    /* Factor n into primes and exponents: primes[i] ^ exps[i].  */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute σ_{23}(n) multiplicatively:
        σ_{23}(p^e) = 1 + p^23 + p^46 + … + p^{23e}.  */
    seq_t sigma = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        /* Compute p^23 for this prime factor. */
        seq_t p_pow23 = (seq_t)1;
        for (int j = 0; j < 23; ++j)
            p_pow23 *= (seq_t)primes[i];

        /* Sum the geometric series for this prime power. */
        seq_t sum = (seq_t)1;          /* first term = 1 */
        seq_t term = p_pow23;          /* p^23 for the 1st exponent i=1 */

        for (size_t e = 1; e <= exps[i]; ++e) {
            sum += term;              /* add p^{23e} */
            term *= p_pow23;          /* prepare next exponent */
        }

        sigma *= sum;
    }

    /* Compute n^23.  */
    seq_t n_pow23 = (seq_t)1;
    for (int j = 0; j < 23; ++j)
        n_pow23 *= (seq_t)n;

    /* Divide out the greatest common divisor. */
    seq_t g = seq_gcd(sigma, n_pow23);
    return sigma / g;
}
