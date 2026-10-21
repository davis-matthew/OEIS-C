/*
 * A057001  (phi(n+1)-phi(n))/2.
 * https://oeis.org/A057001
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (phi(n+1) - phi(n)) / 2
 * Range:     i8 n <= 357, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Compute φ(n) and φ(n+1) using the supplied helper
       and return half their difference.  The subtraction
       is always even, so integer division yields the exact
       value for all n ≥ 1. */
    seq_t diff = phi(n + 1) - phi(n);
    return diff / 2;
}
