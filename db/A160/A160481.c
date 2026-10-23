/*
 * A160481  Row sums of the Beta triangle A160480.
 * https://oeis.org/A160481
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign, uned
 * Formula:   a(n) = (2*n-3)! - 2^(2*n-3)*(n-1)!*(n-2)!
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for n >= 2.  Values for n < 2 are not defined. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* undefined below offset */

    /* Factorial of (n-1)!  (product from 2 to n-1) */
    seq_t fac1 = 1;
    for (idx_t i = 2; i < n; ++i)
        fac1 *= (seq_t)i;

    /* Factorial of (n-2)!  (product from 2 to n-2) */
    seq_t fac0 = 1;
    for (idx_t i = 2; i <= n - 2; ++i)
        fac0 *= (seq_t)i;

    /* Product of numbers from n to (2*n-3) to build (2*n-3)! */
    seq_t fac_range = 1;
    for (idx_t i = n; i <= 2 * n - 3; ++i)
        fac_range *= (seq_t)i;

    /* (2*n-3)! = (n-1)! * product(n..2*n-3) */
    seq_t fac2 = fac1 * fac_range;

    /* 2^(2*n-3) */
    seq_t pow2 = 1;
    idx_t exp = 2 * n - 3;
    for (idx_t i = 0; i < exp; ++i)
        pow2 <<= 1;   /* shift left by 1 multiplies by 2 */

    /* Product (n-1)! * (n-2)! * 2^(2*n-3) */
    seq_t prod = fac1 * fac0;   /* (n-1)! * (n-2)! */
    prod *= pow2;               /* multiply by 2^(2*n-3) */

    /* Final result: (2*n-3)! - prod */
    return fac2 - prod;
}
