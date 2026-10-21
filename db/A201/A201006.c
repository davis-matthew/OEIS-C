/*
 * A201006  The Isis problem: Array a(i,j) (by antidiagonals) of differences
 *          between area and perimeter of rectangle with sides (i,j).
 * https://oeis.org/A201006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = A(i,j) with A(i,j)=i*j-2*(i+j), * where (i,j) are the
 *            antidiagonal coordinates of the n‑th term * in the array
 *            (i>0,j>0)
 * Range:     i8 n <= 337, i16 n <= 9236, i32 n <= 9236, i64 n <= 9236,
 *            i128 n <= 9236, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the diagonal sum s such that
       (s-1)*s/2 >= n.  This diagonal contains the n‑th term. */
    idx_t s = 1;
    while ((s - 1) * s / 2 < n)
        ++s;

    /* Number of terms preceding this diagonal */
    idx_t prev = (s - 2) * (s - 1) / 2;

    /* Zero‑based offset within the diagonal (length s-1) */
    idx_t offset = n - prev - 1;

    /* Antidiagonal coordinates */
    idx_t i = 1 + offset;
    idx_t j = s - i;

    /* Compute the value using seq_t arithmetic */
    seq_t si = (seq_t)i;
    seq_t sj = (seq_t)j;
    seq_t sum = si + sj;
    seq_t prod = si * sj;
    return prod - 2 * sum;
}
