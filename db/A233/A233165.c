/*
 * A233165  Number of n X 5 0..7 arrays with no element x(i,j) adjacent to
 *          itself or value 7-x(i,j) horizontally, diagonally, antidiagonally
 *          or vertically, top left element zero, and 1 appearing before 2 3 4
 *          5 and 6, 2 appearing before 3 4 and 5, and 3 appearing before 4 in
 *          row major order (unlabelled 8-colo...
 * https://oeis.org/A233165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 48; for n > 1, a(n) = 2^(5 n − 6) × (2^n + 28)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety, unused for OEIS offset */
    if (n == 1) return (seq_t)48;    /* first term */

    /* compute exponents for power of two */
    idx_t exp5 = 5 * n - 6;          /* 5*n−6  ≥ 0 for n≥2 */

    /* 2^(5*n-6) */
    seq_t part1 = pow2(exp5);

    /* 2^n */
    seq_t part2 = pow2(n);

    /* a(n) = part1 * (part2 + 28) */
    return part1 * (part2 + (seq_t)28);
}
