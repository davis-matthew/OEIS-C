/*
 * A141388  Triangle T(n, k) = ( k*(n-k+1) )^3 - 2^(n-1), read by rows.
 * https://oeis.org/A141388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, look
 * Formula:   a(n) is the n-th element of triangle T(r,k) = (k*(r-k+1))^3 -
 *            2^(r-1) read by rows, offset 1
 * Range:     i8 n <= 7, i16 n <= 59, i32 n <= 528, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains the nth element */
    idx_t r = 1;
    while ( ((seq_t)r * ((seq_t)(r + 1))) / 2 < n )
        ++r;

    /* Column index within the row */
    idx_t prev_tri = ((r - 1) * r) / 2;
    idx_t col = n - prev_tri;

    /* Compute (k*(r-k+1))^3 */
    seq_t part = (seq_t)col * ((seq_t)(r - col + 1));
    seq_t part2 = part * part;      /* (k*(r-k+1))^2 */
    seq_t part3 = part2 * part;     /* cubic */

    /* Compute 2^(r-1) */
    seq_t pow_two = pow2(r - 1);

    /* Triangle entry: cubic part minus the power of two */
    return part3 - pow_two;
}
