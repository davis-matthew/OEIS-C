/*
 * A193898  Mirror of the triangle A193897.
 * https://oeis.org/A193898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = w(r, r - j) where r is the row index and j = n - r(r+1)/2
 *            and w(r,k) = ((k+1)*(k+2)/2)*(r-k+1) for k < r, w(r,r) =
 *            r(r+1)/2. The triangle at A193898 is the mirror of the triangle
 *            A193897
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special first element */
    if (n == 0)
        return (seq_t)1;

    /* Find the row index r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Offset where this row starts */
    idx_t offset = r * (r + 1) / 2;

    /* Position within the reversed row */
    idx_t j = n - offset;

    /* Original column index in the forward‐indexed triangle */
    idx_t k = r - j;

    /* Compute the triangle value */
    if (k == r) {
        /* w(r,r) = r(r+1)/2 */
        return (seq_t)r * ((seq_t)r + 1) / 2;
    }

    /* w(r,k) = ((k+1)*(k+2)/2)*(r-k+1) */
    idx_t tmp = ((k + 1) * (k + 2) / 2) * (r - k + 1);
    return (seq_t)tmp;
}
