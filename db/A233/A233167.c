/*
 * A233167  Number of n X 7 0..7 arrays with no element x(i,j) adjacent to
 *          itself or value 7-x(i,j) horizontally, diagonally, antidiagonally
 *          or vertically, top left element zero, and 1 appearing before 2 3 4
 *          5 and 6, 2 appearing before 3 4 and 5, and 3 appearing before 4 in
 *          row major order (unlabeled 8-color...
 * https://oeis.org/A233167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1248; for n>1, a(n)=2^(7*n-6)*(2^n+124)
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n<1, but safe guard */
    if (n == 1)
        return (seq_t)1248; /* special case */

    /* compute 2^(7*n-6) */
    seq_t p = pow2(7 * n - 6);

    /* compute (2^n + 124) */
    seq_t q = pow2(n) + (seq_t)124;

    return p * q;
}
