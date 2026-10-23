/*
 * A015431  Gaussian binomial coefficient [ n,12 ] for q=-8.
 * https://oeis.org/A015431
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..12} ((-8)^(n-i+1)-1)/((-8)^i-1) For n < 12
 *            the value is irrelevant. The function uses a short recurrence,
 *            which is exact for every n and works for any seq_t type
 *            (including an arbitrary‑precision integer). It keeps the
 *            intermediate numbers as small as possible by cancelling c...
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 12) return (seq_t)0;        /* values below the offset are ignored */

    /* a(12) = 1 is the starting point */
    seq_t result = (seq_t)1;

    /* current powers of 8:  power8_n = 8^n,  power8_nm12 = 8^(n-12)  */
    seq_t power8_n     = (seq_t)1;
    seq_t power8_nm12  = (seq_t)1;

    /* initialise 8^12 and 8^0 */
    for (idx_t i = 0; i < 12; ++i) power8_n *= (seq_t)8;   /* 8^12 */
    /* power8_nm12 is already 8^0 = 1 */

    /* iterate from n = 13 up to the requested n */
    for (idx_t k = 13; k <= n; ++k) {
        /* update powers to 8^k and 8^(k-12) */
        power8_nm12 *= (seq_t)8;   /* (= 8^(k-12)) */
        power8_n    *= (seq_t)8;   /* (= 8^k) */

        /* absolute numerator: |(-8)^k - 1| */
        seq_t term_num = ( (k & 1) ? power8_n + (seq_t)1
                                   : power8_n - (seq_t)1 );

        /* absolute denominator: |(-8)^(k-12) - 1| */
        idx_t exp_den = k - 12;    /* >= 1 */
        seq_t term_den = ( (exp_den & 1) ? power8_nm12 + (seq_t)1
                                         : power8_nm12 - (seq_t)1 );

        /* cancel common factors between the new term and the existing result */
        seq_t g = gcd(term_num, term_den);
        if (g != (seq_t)1) {
            term_num /= g;
            term_den /= g;
        }

        /* reduce result with the new denominator */
        g = gcd(result, term_den);
        if (g != (seq_t)1) {
            result   /= g;
            term_den /= g;
        }

        /* multiply the remaining numerator and divide by the remaining denominator */
        result *= term_num;
        /* term_den should now divide result exactly */
        result /= term_den;
    }

    return result;
}
