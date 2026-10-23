/*
 * A015333  Gaussian binomial coefficient [ n,6 ] for q = -10.
 * https://oeis.org/A015333
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{i=1..6} |(-10)^{n+1-i} - 1| / |(-10)^i - 1|
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the Gaussian binomial coefficient [n,6] for q = -10. */

#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /*  For n < 6 the coefficient does not exist (k > n).  */
    if (n < 6) return (seq_t)0;

    const int k = 6;
    seq_t num[k];
    seq_t den[k];

    /*  Build absolute values of numerator and denominator factors.  */
    for (int i = 1; i <= k; ++i) {
        /*  Numerator factor: |(-10)^{n+1-i} - 1|   */
        idx_t p = n + 1 - (idx_t)i;
        seq_t p10 = pow10(p);
        if ((p & 1) == 0)   /* even exponent: 10^p - 1  */
            num[i - 1] = p10 - (seq_t)1;
        else                /* odd exponent: 10^p + 1   */
            num[i - 1] = p10 + (seq_t)1;

        /*  Denominator factor: |(-10)^i - 1|      */
        idx_t q = (idx_t)i;
        seq_t q10 = pow10(q);
        if ((q & 1) == 0)   /* even exponent: 10^q - 1  */
            den[i - 1] = q10 - (seq_t)1;
        else                /* odd exponent: 10^q + 1   */
            den[i - 1] = q10 + (seq_t)1;
    }

    /*  Cancel common factors between numerators and denominators
        to avoid overflow in intermediate products.  */
    for (int i = 0; i < k; ++i) {
        seq_t d = den[i];
        if (d <= (seq_t)1) continue;          /* 1 or 0 needs no cancellation */

        for (int j = 0; j < k; ++j) {
            seq_t g = gcd(num[j], d);
            if (g <= (seq_t)1) continue;
            num[j] /= g;
            d     /= g;
            if (d <= (seq_t)1) break;
        }
        /*  Any remaining divisor should be 1 for a correct term.  */
    }

    /*  Multiply the surviving numerator factors.  */
    seq_t res = (seq_t)1;
    for (int i = 0; i < k; ++i)
        if (num[i] != (seq_t)1)
            res *= num[i];

    return res;
}
