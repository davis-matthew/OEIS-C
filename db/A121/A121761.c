/*
 * A121761  In decimal number system, take negative power of 10 at even digits
 *          of n. Sequence gives numerators of result.
 * https://oeis.org/A121761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, frac, nonn
 * Formula:   a(n) = numerator of the rational number obtained by * replacing
 *            even decimal digits of n with negative powers of 10 * and odd
 *            digits with positive powers of 10. The fraction is * reduced to
 *            lowest terms and the numerator is returned
 * Range:     u8 n <= 119, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"   /* pow10(idx_t e) -> seq_t 10^e */
#include "../utils/gcd.h"     /* gcd(seq_t a, seq_t b) -> seq_t */

seq_t a(idx_t n)
{
    /* Find the highest position (0‑based, units = 0) that contains an even digit. */
    idx_t temp = n;
    idx_t pos = 0;
    idx_t Dmax = 0;
    int digit;

    while (temp > 0) {
        digit = (int)(temp % 10);
        if ((digit & 1) == 0) {         /* even digit */
            if (pos > Dmax)
                Dmax = pos;
        }
        temp /= 10;
        pos += 1;
    }

    /* Compute the scaled numerator with a common denominator 10^Dmax. */
    temp = n;
    pos = 0;
    seq_t sum = 0;

    while (temp > 0) {
        digit = (int)(temp % 10);
        if ((digit & 1) == 0) {           /* even digit -> negative power */
            sum += (seq_t)digit * pow10(Dmax - pos);
        } else {                          /* odd digit -> positive power */
            sum += (seq_t)digit * pow10(pos + Dmax);
        }
        temp /= 10;
        pos += 1;
    }

    /* Reduce the fraction sum / 10^Dmax and return the numerator. */
    seq_t denom = pow10(Dmax);
    seq_t g = gcd(sum, denom);
    sum /= g;
    return sum;
}
