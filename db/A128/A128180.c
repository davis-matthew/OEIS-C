/*
 * A128180  Triangle read by rows: A002260 * A097807 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A128180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = ((2k-1) + (-1)^(r-k)*(2r+1)) / 4 where r = smallest
 *            integer with r*(r+1)/2 >= n, k = n - r*(r-1)/2
 * Range:     i8 n <= 1274, i16 n <= 1275, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t cum = 1;               /* r*(r+1)/2 for the current r */
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t prev = cum - r;        /* r*(r-1)/2 */
    idx_t k = n - prev;          /* column index (1‑based) */

    /* compute the triangle element T(r,k) */
    seq_t sign = ((r - k) & 1ULL) ? (seq_t)-1 : (seq_t)1;        /* (-1)^(r-k) */
    seq_t term = ((seq_t)2 * (seq_t)k - (seq_t)1) + sign * (((seq_t)2 * (seq_t)r) + (seq_t)1);
    return term / (seq_t)4;
}
