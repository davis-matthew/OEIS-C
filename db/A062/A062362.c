/*
 * A062362  a(n) = floor of Sum_{ d divides n } phi(d)/d.
 * https://oeis.org/A062362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( sum_{d|n} phi(d)/d ) * = floor( ∏_{p^e||n} (1 +
 *            e*(1-1/p)) ) * * We factor n, then compute the product of the
 *            terms * (p + e*(p-1))/p for each prime power p^e. * The fraction
 *            is reduced at every step with gcd, * ensuring the intermediate
 *            numerators/denominators stay * within the sup...
 * Range:     u8 n <= 69, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle n = 1 separately – product over empty set equals 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Prime factorisation ------------------------------------------------- */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the product of (p + e*(p-1))/p, reducing each time. -------- */
    seq_t num = (seq_t)1;   /* numerator   */
    seq_t den = (seq_t)1;   /* denominator */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p  = (seq_t)primes[i];
        seq_t e  = (seq_t)exps[i];
        seq_t numer_i = p + e * (p - (seq_t)1);  /* p + e*(p-1)   */
        seq_t denom_i = p;                        /* p             */

        num = num * numer_i;
        den = den * denom_i;

        /* Reduce the fraction to avoid overflow. */
        seq_t g = gcd(num, den);
        if (g > (seq_t)1) {
            num = num / g;
            den = den / g;
        }
    }

    /* Floor of the rational number is just integer division. */
    return num / den;
}
