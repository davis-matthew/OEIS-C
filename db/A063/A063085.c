/*
 * A063085  a(n) = usigma(n) - (phi(n) + d(n)), where usigma(n) is the sum of
 *          the unitary divisors of n and d(n) is the number of divisors of n.
 * https://oeis.org/A063085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = unitary_sigma(n) - (phi(n) + count_divisors(n))
 * Range:     i8 n <= 77, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   count_divisors, phi, unitary_sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/unitary_sigma.h"
#include "../utils/phi.h"
#include "../utils/count_divisors.h"

seq_t a(idx_t n)
{
    /* n >= 1 by sequence definition */
    seq_t us = unitary_sigma(n);
    seq_t ph = phi(n);
    seq_t d  = count_divisors(n);
    return us - (ph + d);
}
