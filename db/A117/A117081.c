/*
 * A117081  a(n) = 36*n^2 - 810*n + 2753, producing the conjectured record
 *          number of 45 primes in a contiguous range of n for quadratic
 *          polynomials, i.e., abs(a(n)) is prime for 0 <= n < 44.
 * https://oeis.org/A117081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = 36*n^2 - 810*n + 2753
 * Range:     i8 none, i16 n <= 42, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 36 * nn * nn - 810 * nn + 2753;
}
