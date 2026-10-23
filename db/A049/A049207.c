/*
 * A049207  Array T(m,n) of products of pronic numbers m(m+1) * n(n+1) read by
 *          antidiagonals ("bipronics").
 * https://oeis.org/A049207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = m(m+1) * n(n+1) read along antidiagonals of the array
 *            T(m,n) = m(m+1) * n(n+1), with (m,n) determined by the
 *            triangular decomposition of n. The index n counts the elements
 *            along successive antidiagonals, starting with n = 0
 * Range:     u8 n <= 38, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    /* Find k so that k*(k+1)/2 <= n < (k+1)*(k+2)/2. */
    idx_t k = 0;
    while (((k + 1) * (k + 2)) / 2 <= n)
        ++k;

    /* Position within diagonal k. */
    idx_t offset = n - (k * (k + 1)) / 2;
    idx_t i = offset;          /* row index   */
    idx_t j = k - offset;      /* column index */

    /* Compute the product of the corresponding pronic numbers. */
    seq_t pronic_i = (seq_t)i * ((seq_t)i + 1);
    seq_t pronic_j = (seq_t)j * ((seq_t)j + 1);
    return pronic_i * pronic_j;
}
