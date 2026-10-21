/*
 * A233166  Number of n X 6 0..7 arrays with no element x(i,j) adjacent to
 *          itself or value 7-x(i,j) horizontally, diagonally, antidiagonally
 *          or vertically, top left element zero, and 1 appearing before 2 3 4
 *          5 and 6, 2 appearing before 3 4 and 5, and 3 appearing before 4 in
 *          row major order (unlabelled 8-colo...
 * https://oeis.org/A233166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=236; for n>1, a(n) = 64^(n-1) * (2^n + 60)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)236;

    idx_t exp1 = 6 * (n - 1);          /* 6*(n-1)  -> exponent for 64^(n-1) */
    seq_t part1 = pow2(exp1);          /* 2^(6*(n-1)) = 64^(n-1) */

    seq_t part2 = pow2(n);             /* 2^n */
    part2 += (seq_t)60;                /* 2^n + 60 */

    return part1 * part2;              /* 64^(n-1) * (2^n + 60) */
}
