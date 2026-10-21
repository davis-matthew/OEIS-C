/*
 * A099398  Numerators of rationals (in lowest terms) used in a certain high
 *          temperature expansion.
 * https://oeis.org/A099398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator( Catalan(n) / ((n+2) * 2^(2*n-1)) )
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute binomial(2*n, n), cancel common factors *(n+1) and *(n+2),
   then cancel the required powers of two. */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* binomial(2*n, n) */
    seq_t num = binomial(2 * n, n);

    /* cancel common factors of (n+1) and (n+2) */
    seq_t g = gcd(num, (seq_t)(n + 1));
    num /= g;

    g = gcd(num, (seq_t)(n + 2));
    num /= g;

    /* cancel powers of two up to exponent 2*n-1 */
    int twoExp = 2 * (int)n - 1;      /* for n>=1 this is >=0 */
    int cancel = 0;
    while (cancel < twoExp && (num & 1) == 0)
    {
        num >>= 1;
        ++cancel;
    }

    return num;                       /* numerator of reduced fraction */
}
