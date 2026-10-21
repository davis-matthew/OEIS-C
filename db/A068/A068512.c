/*
 * A068512  Numerators of arithmetic derivative of n/2: A003415(n)/2 - n/4;
 *          denominators: A010685.
 * https://oeis.org/A068512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (A003415(n)/2 - n/4) reduced, returning the numerators of
 *            the fraction A003415(n)/2 - n/4. The implementation uses the
 *            arithmetic derivative function arithmetic_derivative(n) and
 *            simplifies the resulting rational with denominator 4 by dividing
 *            out the gcd with 4
 * Range:     i8 n <= 31, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   arithmetic_derivative, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/arithmetic_derivative.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute the arithmetic derivative d(n). */
    seq_t d = arithmetic_derivative((uint64_t)n);   /* d is signed */

    /* numerator = 2*d - n, denom = 4. */
    seq_t numer = (d << 1) - (seq_t)n;
    seq_t denom = (seq_t)4;

    /* Reduce the fraction by dividing out the gcd of |numer| and 4. */
    seq_t abs_numer = numer >= 0 ? numer : -numer;
    seq_t g = gcd(abs_numer, denom);

    /* Return the reduced numerator. */
    return numer / g;
}
