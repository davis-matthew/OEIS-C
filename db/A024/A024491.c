/*
 * A024491  a(n) = (1/(4n-1))*C(4n,2n).
 * https://oeis.org/A024491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = binomial(4n,2n)/(4n-1)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence 
   a(0) = -1
   a(n) = a(n-1) * 2*(4n-3)*(4n-5) / [ n*(2n-1) ]   for n >= 1
   The product is reduced by GCDs to avoid overflow. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return -1;

    seq_t result = -1;

    for (idx_t k = 1; k <= n; ++k) {
        /* convert k to seq_t once */
        seq_t kseq = (seq_t)k;

        /* numerator: 2 * (4k-3) * (4k-5) */
        seq_t num = 2 * ((kseq * 4 - 3) * (kseq * 4 - 5));

        /* denominator: n * (2n-1) */
        seq_t den = kseq * (2 * kseq - 1);

        /* cancel common factors with result and denominator */
        seq_t g1 = gcd(result, den);
        result /= g1;
        den /= g1;

        /* cancel common factors with numerator and remaining denominator */
        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;

        /* multiply and divide */
        result *= num;
        result /= den;
    }

    return result;
}
