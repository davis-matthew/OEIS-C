/*
 * A233161  Number of n X n 0..7 arrays with no element x(i,j) adjacent to
 *          itself or value 7-x(i,j) horizontally, diagonally, antidiagonally
 *          or vertically, top left element zero, and 1 appearing before 2 3 4
 *          5 and 6, 2 appearing before 3 4 and 5, and 3 appearing before 4 in
 *          row major order (unlabelled 8-colo...
 * https://oeis.org/A233161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(n-1)-1) * 2^(n^2-4) (for n ≥ 2) a(1) = 1 (Diagonal of
 *            A233168)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"   /* pow2(idx_t exp) -> seq_t */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* 2^(n-1) */
    seq_t two_n_minus1 = pow2((idx_t)(n - 1));

    /* (2^(n-1) - 1) */
    seq_t factor = two_n_minus1 - (seq_t)1;

    /* exponent n^2 - 4 */
    idx_t exp = n * n;
    exp -= (idx_t)4;

    /* 2^(n^2-4) */
    seq_t pow_factor = pow2(exp);

    /* final product */
    return factor * pow_factor;
}
