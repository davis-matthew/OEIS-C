/*
 * A098985  Denominators in series expansion of log( Sum_{m=-oo,oo} q^(m^2) ).
 * https://oeis.org/A098985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = denominator of \sum_{odd d\mid n} 1/d , a(0)=1 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;          // offset zero
    /* strip factors of 2: they do not influence odd divisors */
    idx_t m = n;
    while ((m & 1ULL) == 0) m >>= 1;
    if (m == 1) return 1;          // only divisor is 1

    seq_t num = 1;                 /* rational product numerator   */
    seq_t den = 1;                 /* rational product denominator */

    /* trial division up to sqrt(m) */
    idx_t temp = m;
    for (idx_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            idx_t exp = 0;
            while (temp % p == 0) { temp /= p; ++exp; }

            /* p^k and p^(k+1) in seq_t */
            seq_t pk = 1;
            for (idx_t i = 0; i < exp; ++i) pk *= (seq_t)p;
            seq_t pk1 = pk * (seq_t)p;            /* p^(k+1) */

            /* fraction for this prime power:
               (p^(k+1)-1) / (p^k * (p-1))  */
            seq_t fnum = pk1 - 1;
            seq_t fden = pk * (seq_t)(p - 1);

            /* multiply into the running product */
            num = num * fnum;
            den = den * fden;

            /* reduce by gcd to keep sizes small */
            seq_t g = gcd(num, den);
            num /= g;
            den /= g;
        }
    }

    /* if a prime factor remains > sqrt(original m) */
    if (temp > 1)
    {
        idx_t p = temp;              /* exponent = 1 */
        seq_t pk = (seq_t)p;
        seq_t pk1 = pk * (seq_t)p;
        seq_t fnum = pk1 - 1;
        seq_t fden = pk * (seq_t)(p - 1);

        num = num * fnum;
        den = den * fden;

        seq_t g = gcd(num, den);
        num /= g;
        den /= g;
    }

    return den;
}
