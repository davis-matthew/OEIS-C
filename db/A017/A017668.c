/*
 * A017668  Denominator of sum of -2nd powers of divisors of n.
 * https://oeis.org/A017668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^2 / gcd(n^2, σ₂(n)) where σ₂(n) = Σ_{d|n} d²
 *            (Denominator of the reduced fraction Σ_{d|n} 1/d²)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 2551, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* Offset is 1; n=0 is unused */

    uint64_t nn = (uint64_t)n;

    /* Factorise n: primes[i] raised to exps[i] for i=0..cnt-1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    /* Compute σ₂(n) multiplicatively */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];          /* exponent of prime p */

        /* powp = p^(2*(e+1)) */
        seq_t powp = 1;
        uint64_t exp_val = 2 * e + 2;   /* equals 2*(e+1) */
        for (uint64_t k = 0; k < exp_val; ++k)
            powp *= p;

        seq_t numerator   = powp - 1;          /* p^(2*(e+1)) - 1 */
        seq_t denom_term  = p * p - 1;         /* p² - 1            */
        seq_t temp        = sigma * numerator; /* intermediate product  */
        temp /= denom_term;                   /* multiply term of σ₂ */
        sigma = temp;
    }

    /* n² and the required gcd */
    seq_t n2 = (seq_t)nn * (seq_t)nn;
    seq_t g  = gcd(sigma, n2);

    return n2 / g;    /* denominator of the reduced sum */
}
