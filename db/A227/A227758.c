/*
 * A227758  a(n) = sigma(sigma(n)) - sigma(n) - n, where sigma(n) = A000203(n)
 *          = sum of the divisors of n.
 * https://oeis.org/A227758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sigma(sigma(n)) - sigma(n) - n
 * Range:     i8 n <= 53, i16 n <= 4319, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   sigma_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_generic.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t s      = sigma_generic(n_seq);   /* sigma(n)           */
    seq_t ss     = sigma_generic(s);       /* sigma(sigma(n))    */
    return ss - s - n_seq;                 /* a(n)               */
}
