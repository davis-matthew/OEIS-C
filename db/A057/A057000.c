/*
 * A057000  a(n) = phi(n+1) - phi(n).
 * https://oeis.org/A057000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = phi(n+1) - phi(n)
 * Range:     i8 n <= 177, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_phi.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    idx_t n_plus_1 = n + 1;
    return _phi(n_plus_1) - _phi(n);
}
