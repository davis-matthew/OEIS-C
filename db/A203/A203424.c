/*
 * A203424  Reciprocal of Vandermonde determinant of (1/2,1/4,...,1/(2n)).
 * https://oeis.org/A203424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = \prod_{k=1}^{n} (-2k)^{k-1}
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 6, i128 n <= 8,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, but safe guard */
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t base = - (seq_t)(2 * k);    /* (-2k) */
        seq_t term = pow_seq(base, k - 1);
        result *= term;
    }
    return result;
}
