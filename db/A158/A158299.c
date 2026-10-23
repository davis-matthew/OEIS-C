/*
 * A158299  Numerators of averages of squares of the divisors of n.
 * https://oeis.org/A158299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of ( σ₂(n) / τ(n) ) in lowest terms where σ₂(n)
 *            = sum of squares of the divisors of n and τ(n) = number of
 *            divisors of n. The function returns the numerator of the reduced
 *            fraction Σ d² / τ(n)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 330, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* factor the integer n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* start with σ₂(n)=1 and τ(n)=1 */
    seq_t sigma2 = 1;
    seq_t tau    = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t pseq = (seq_t)p;

        /* term = (p^(2(e+1)) - 1) / (p² - 1)   */
        seq_t p2   = pseq * pseq;                 /* p²  */
        seq_t num  = big_pow(p2, (seq_t)(e + 1)) - 1; /* p²(e+1) - 1 */
        seq_t den  = p2 - 1;                        /* p² - 1   */
        seq_t term = num / den;                     /* integer */

        sigma2 *= term;
        tau    *= (seq_t)(e + 1);
    }

    /* reduce the fraction and return the numerator */
    seq_t g = gcd(sigma2, tau);
    return sigma2 / g;
}
