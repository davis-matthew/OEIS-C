/*
 * A103339  Numerator of the unitary harmonic mean (i.e., the harmonic mean of
 *          the unitary divisors) of the positive integer n.
 * https://oeis.org/A103339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of the unitary harmonic mean of n. For a prime
 *            factorization n = ∏ p_i^{e_i}, let k be the number of distinct
 *            primes. Unitry divisors are products of either 1 or p_i^{e_i}.
 *            Sum of reciprocals = ∏ (1 + 1/p_i^{e_i}) = ∏ (p_i^{e_i}+1)/∏
 *            p_i^{e_i}. H = (2^k) / [∑ 1/d] = (2^k * ∏ p_i^...
 * Range:     u8 n <= 41, u16 n <= 4549, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)  // defined only for n>=1, but guard anyway
        return 0;

    idx_t temp = n;          /* copy for factorization   */
    idx_t k = 0;              /* number of distinct primes*/
    seq_t prod_num = 1;       /* ∏ p_i^{e_i}             */
    seq_t prod_den = 1;       /* ∏ (p_i^{e_i}+1)         */

    /* factor 2 separately to keep loop step 2 */
    if ((temp & 1) == 0) {
        k++;
        seq_t p_pow = 1;
        while ((temp & 1) == 0) {
            temp >>= 1;
            p_pow <<= 1;          /* multiply by 2 */
        }
        prod_num *= p_pow;
        prod_den *= (p_pow + 1);
    }

    /* remaining odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            k++;
            seq_t p_pow = 1;
            while (temp % p == 0) {
                temp /= p;
                p_pow *= p;
            }
            prod_num *= p_pow;
            prod_den *= (p_pow + 1);
        }
    }

    /* if a prime factor remains */
    if (temp > 1) {
        k++;
        seq_t p_pow = temp;        /* remaining prime ^1   */
        prod_num *= p_pow;
        prod_den *= (p_pow + 1);
    }

    /* U = 2^k      */
    seq_t U = ((seq_t)1) << k;

    /* H = U * prod_num / prod_den   */
    seq_t numerator = prod_num * U;
    seq_t denominator = prod_den;

    /* reduce fraction */
    seq_t g = gcd_seq(numerator, denominator);
    numerator /= g;

    return numerator;
}
