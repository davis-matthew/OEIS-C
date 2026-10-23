/*
 * A128766  Number of inequivalent n-colorings of the vertices of the 3D cube
 *          under full orthogonal group of the cube (of order 48).
 * https://oeis.org/A128766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/48)*(20*n^2 + 21*n^4 + 6*n^6 + n^8)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* convert index to sequence type */
    seq_t x = (seq_t)n;
    /* compute powers */
    seq_t n2 = x * x;          /* n^2 */
    seq_t n4 = n2 * n2;         /* n^4 */
    seq_t n6 = n4 * n2;         /* n^6 */
    seq_t n8 = n4 * n4;         /* n^8 */
    /* polynomial numerator */
    seq_t term = (seq_t)20 * n2;
    term = term + (seq_t)21 * n4;
    term = term + (seq_t)6 * n6;
    term = term + n8;
    /* divide by 48 */
    return term / (seq_t)48;
}
