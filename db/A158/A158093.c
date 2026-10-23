/*
 * A158093  a(n) = 3^(n^2+n)*C(1/3^n, n) = [x^n] (1 + 3^(n+1)*x)^(1/3^n).
 * https://oeis.org/A158093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3^n * ∏_{i=0}^{n-1} (1 - i*3^n) / n!
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 9,
 *            big any n
 * Helpers:   gcd, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(0) = 1.  Uses arbitrary precision integer operations. */

#include "../utils/pow3.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the offset n = 0 explicitly */
    if (n == 0)
        return 1;

    /* 3^n as a seq_t */
    seq_t pow3n = pow3(n);

    /* Numerator and denominator for the binomial part */
    seq_t num   = 1;   /* will accumulate ∏ (1 - i*3^n) */
    seq_t denom = 1;   /* will accumulate i! */

    /* Compute the product part while cancelling with the factorial */
    for (idx_t i = 1; i <= n; ++i) {
        /* term = 1 - (i-1)*3^n */
        seq_t term = pow3n * (seq_t)(i - 1); /* (i-1)*3^n */
        term = 1 - term;

        num   *= term;        /* multiply by the term */
        denom *= (seq_t)i;    /* multiply by i for factorial */

        /* Reduce gcd(num, denom) to keep numbers as small as possible */
        seq_t g = gcd(num < 0 ? -num : num, denom);
        if (g > 1) {
            num   /= g;
            denom /= g;
        }
    }

    /* Now a(n) = (num * 3^n) / denom */
    /* Cancel any common factors between pow3n and denom first */
    seq_t g3 = gcd(pow3n, denom);
    if (g3 > 1) {
        pow3n /= g3;
        denom /= g3;
    }

    /* Multiply numerator by the remaining power of 3 */
    num *= pow3n;

    /* Final division – result is guaranteed to be integer */
    seq_t result = num / denom;

    return result;
}
