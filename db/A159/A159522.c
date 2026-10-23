/*
 * A159522  Numerator of Hermite(n, 3/16).
 * https://oeis.org/A159522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (-1)^k * n! * 3^{n-2k} * 8^{2k} /
 *            (k! * (n-2k)!)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Special handling for n = 0 */
    if (n == 0) return (seq_t)1;

    idx_t half = n / 2;          /* floor(n/2) */

    /* term for k = 0 : 3^n */
    seq_t pow3 = seq_pow((seq_t)3, n);   /* 3^n as seq_t */
    seq_t pow64 = (seq_t)1;              /* 64^0 = 1 */
    seq_t B = (seq_t)1;                  /* binomial factor n! / (0! * n!) = 1 */
    seq_t sum = B * pow3 * pow64;        /* first term */

    for (idx_t k = 1; k <= half; ++k)
    {
        /* Update binomial factor:
           B = B * (n-2k+2)*(n-2k+1) / k
         */
        idx_t factor1 = n - 2 * k + 2;
        idx_t factor2 = n - 2 * k + 1;
        seq_t numer = B * (seq_t)factor1 * (seq_t)factor2;
        B = numer / (unsigned long long)k;  /* exact division */

        /* Update powers
           pow3 = pow3 / 9  (since exponent decreases by 2)
           pow64 = pow64 * 64
         */
        pow3 = pow3 / (unsigned long long)9;
        pow64 = pow64 * (seq_t)64;

        /* Compute current term */
        seq_t term = B * pow3 * pow64;
        if (k & 1) term = -term;  /* alternate sign */
        sum += term;
    }

    return sum;
}
