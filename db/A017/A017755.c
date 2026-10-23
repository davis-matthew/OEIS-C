/*
 * A017755  Binomial coefficients C(n,91).
 * https://oeis.org/A017755
 *
 * Offset:    91    (a(91) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, 91) for n >= 91, a(n) = 0 otherwise
 * Range:     u8 n <= 92, u16 n <= 93, u32 n <= 97, u64 n <= 107,
 *            u128 n <= 140, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 91, so values for n < 91 are not defined. */
    if (n < 91)
        return 0;

    const idx_t k = 91;
    seq_t result = 1;          /* the running binomial product */

    for (idx_t i = 1; i <= k; ++i)
    {
        idx_t numerator   = n - k + i;   /* current factor (n-90)..n */
        idx_t denominator = i;           /* divide by i at each step */

        /* cancel common factors between numerator and denominator */
        seq_t g1 = gcd((seq_t)numerator, (seq_t)denominator);
        numerator   /= (idx_t)g1;
        denominator /= (idx_t)g1;

        /* now cancel any remaining divisor from the running result */
        seq_t g2 = gcd(result, (seq_t)denominator);
        result /= g2;
        denominator /= (idx_t)g2;         /* should become 1  */

        /* after complete reduction, denominator must be 1 */
        /* multiply the remaining numerator into the running product */
        result *= (seq_t)numerator;
    }

    return result;
}
