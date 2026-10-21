/*
 * A062270  Numerators in partial products of the twin prime constant.
 * https://oeis.org/A062270
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)*p_n*(p_n-2) / gcd(a(n-1)*p_n*(p_n-2), P(n))
   where p_n is the n-th prime and P(n)=∏_{k=2}^n (p_k-1)^2.
   The implementation keeps a reduced fraction (numerator/denominator)
   and reduces after each step to avoid overflow.  For n<2 the
   value is undefined; here we return 0. */
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;           /* undefined for offset 2 */

    seq_t num = (seq_t)1;                 /* current numerator   */
    seq_t den = (seq_t)1;                 /* current denominator */

    /* iterate over primes from 2nd up to n-th */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t p = (seq_t)get_prime(k);          /* k-th prime */

        /* factors for this step */
        seq_t mul_num = p * (p - (seq_t)2);    /* p*(p-2)    */
        seq_t mul_den = (p - (seq_t)1);        /* (p-1)      */
        mul_den = mul_den * mul_den;           /* (p-1)^2    */

        /* remove factors common to mul_num and the existing denominator */
        seq_t g1 = gcd(mul_num, den);
        mul_num /= g1;
        den     /= g1;

        /* remove factors common to mul_den and the existing numerator */
        seq_t g2 = gcd(num, mul_den);
        num     /= g2;
        mul_den  /= g2;

        /* multiply the reduced factors */
        num *= mul_num;
        den *= mul_den;
        /* no further gcd needed – the fractions are already reduced */
    }

    return num;   /* denominator should be 1 at this point */
}
