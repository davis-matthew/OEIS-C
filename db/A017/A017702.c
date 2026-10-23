/*
 * A017702  Denominator of sum of -19th powers of divisors of n.
 * https://oeis.org/A017702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of \sum_{d|n} 1/d^{19} = n^{19} /
 *            gcd(\sigma_{19}(n), n^{19})
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 10, u128 n <= 106,
 *            big any n
 * Helpers:   big_pow, gcd, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_k.h"   /* seq_t sigma_k(idx_t m, uint8_t k)       */
#include "../utils/gcd.h"      /* seq_t gcd(seq_t a, seq_t b)           */
#include "../utils/big_pow.h"  /* seq_t big_pow(seq_t base, seq_t exp)  */

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n<1, return 0 safely     */
        return (seq_t)0;

    /* sigma_19(n) = sum_{d|n} d^19                             */
    seq_t sigma = sigma_k(n, (uint8_t)19);

    /* n^19                                                     */
    seq_t denom = big_pow((seq_t)n, (seq_t)19ULL);

    /* common divisor                                          */
    seq_t g = gcd(sigma, denom);

    /* reduced denominator                                    */
    return denom / g;
}
