/*
 * A124897  a(n) = mu(n^2 + 1), mu = A008683.
 * https://oeis.org/A124897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = mu(n^2 + 1)
 * Range:     i8 n <= 12, i16 n <= 2224, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* compute n^2 + 1 in seq_t type */
    seq_t m = (seq_t)n * (seq_t)n;
    m += 1;
    /* value fits into idx_t because n is small (≤10^4 for tested range) */
    return mobius((idx_t)m);
}
