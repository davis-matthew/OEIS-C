/*
 * A172526  a(n)=floor(3*n^2*(2+sqrt(3))).
 * https://oeis.org/A172526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(3*n^2*(2+sqrt(3)))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* For safety handle n==0, though offset starts at 1 */
    if (n == 0)
        return 0;

    /* n^2 in seq_t */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    /* n^4 = (n^2)^2 */
    seq_t n_four = n_sq * n_sq;

    /* 27 * n^4   (equivalent to (3*sqrt(3)*n^2)^2) */
    seq_t prod = n_four * 27;

    /* floor(sqrt(27 * n^4)) = floor(3*sqrt(3) * n^2) */
    seq_t part = floor_sqrt(prod);

    /* Final result: 6*n^2 + floor(3*sqrt(3) * n^2) */
    return n_sq * 6 + part;
}
