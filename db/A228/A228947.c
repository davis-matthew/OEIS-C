/*
 * A228947  a(n) = sigma(n) - phi(n) - n.
 * https://oeis.org/A228947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sigma(n) - phi(n) - n
 * Range:     i8 n <= 61, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* a(1) = -1, a(2) = 0, ... */
    seq_t s = sigma_of(n);
    seq_t p = phi(n);
    return s - p - (seq_t)n;
}
