/*
 * A226983  a(n) = ceiling(n/2) - pi(2n) + pi(n-1).
 * https://oeis.org/A226983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ceil(n/2) - pi(2n) + pi(n-1)
 * Range:     i8 n <= 136, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* ceil(n/2) == (n + 1) / 2 for unsigned integers */
    seq_t ceil_n2 = (seq_t)(n + 1) / 2;

    /* 2*n without overflow for the intended range of n */
    idx_t two_n = n + n;

    /* prime counting function values */
    seq_t pi_2n  = compute_pi(two_n);
    seq_t pi_n1  = compute_pi(n - 1);

    /* compute the sequence value */
    return ceil_n2 - pi_2n + pi_n1;
}
