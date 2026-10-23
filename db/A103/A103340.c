/*
 * A103340  Denominator of the unitary harmonic mean (i.e., the harmonic mean
 *          of the unitary divisors) of the positive integer n.
 * https://oeis.org/A103340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of the unitary harmonic mean of n
 * Range:     u8 n <= 41, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   gcd_any
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_any.h"
/* For n = ∏ p_i^{e_i}:  a(n) = (∏(p_i^{e_i}+1))/gcd(n·2^k,∏(p_i^{e_i}+1)),
   where k is the number of distinct primes. */

seq_t a(idx_t n)
{
    /* factor n and build the product of (p^e + 1) */
    idx_t temp = n;
    unsigned int k = 0;             /* distinct prime count */
    seq_t divisor = 1;              /* ∏(p^e + 1) */

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            ++k;
            seq_t p_pow = 1;
            while (temp % p == 0) {
                temp /= p;
                p_pow *= (seq_t)p;
            }
            divisor *= (p_pow + 1);
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        ++k;
        divisor *= ((seq_t)temp + 1);
    }

    /* numerator = n * 2^k */
    seq_t two_pow_k = 1;
    for (unsigned int i = 0; i < k; ++i)
        two_pow_k <<= 1;

    seq_t numerator = (seq_t)n * two_pow_k;

    /* reduce fraction to obtain the denominator */
    seq_t g = gcd_any(numerator, divisor);
    return divisor / g;
}
