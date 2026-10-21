/*
 * A085340  a(n) is the value of determinant of the following special matrix:
 *          diagonal values equal to n-2; upper triangular entries equal to
 *          -1; lower triangular values are +1.
 * https://oeis.org/A085340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ((n-1)^n + (n-3)^n) / 2 for n >= 1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


/* Exponentiation by squaring for arbitrary-precision or native seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) {
            result *= b;
        }
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* base1 = n-1, base2 = n-3 ; use signed arithmetic */
    seq_t base1 = (seq_t)n - 1;
    seq_t base2 = (seq_t)n - 3;

    seq_t pow1 = pow_seq(base1, n);
    seq_t pow2 = pow_seq(base2, n);

    /* the sum is always even */
    return (pow1 + pow2) / 2;
}
