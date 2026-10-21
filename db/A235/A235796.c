/*
 * A235796  2*n - 1 - sigma(n).
 * https://oeis.org/A235796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*n - 1 - sigma(n)
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Ensure n is converted to seq_t to avoid mixed integer arithmetic */
    seq_t two_n = ((seq_t)n) << 1;          /* 2*n */
    seq_t sigma = sigma_of(n);              /* sum of divisors of n */
    return two_n - 1 - sigma;
}
