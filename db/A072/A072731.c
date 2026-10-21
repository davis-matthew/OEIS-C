/*
 * A072731  Difference of numbers of composite and prime numbers <= n.
 * https://oeis.org/A072731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - 2*pi(n) - 1
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    seq_t pi = compute_pi(n);
    seq_t res = (seq_t)n - (seq_t)2 * pi - (seq_t)1;
    return res;
}
