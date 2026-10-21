/*
 * A064169  Numerator - denominator in n-th harmonic number, 1 + 1/2 + 1/3 +
 *          ... + 1/n.
 * https://oeis.org/A064169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = numerator(H_n) - denominator(H_n) where H_n = 1 + 1/2 +
 *            ... + 1/n and the fraction is reduced to lowest terms
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 24, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The fraction H_n is stored as num/den in lowest terms.  */
    seq_t num = (seq_t)1;   /* numerator of H_1 (i.e. 1) */
    seq_t den = (seq_t)1;   /* denominator of H_1 */

    for (idx_t k = (idx_t)2; k <= n; ++k)
    {
        /* H_k = H_{k-1} + 1/k = num/den + 1/k */
        seq_t g = gcd(den, (seq_t)k);          /* g = gcd(den, k)  */
        seq_t n1 = num * ((seq_t)k / g);       /* parts that fit in seq_t */
        seq_t d1 = den / g;
        seq_t new_num = n1 + d1;               /* num*k/g + den/g   */
        seq_t new_den = den * ((seq_t)k / g);  /* lcm(den,k)        */

        /* Reduce the new fraction to lowest terms. */
        seq_t h = gcd(new_num, new_den);
        num = new_num / h;
        den = new_den / h;
    }

    return num - den;
}
