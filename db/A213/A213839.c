/*
 * A213839  Principal diagonal of the convolution array A213838.
 * https://oeis.org/A213839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n - 21*n^2 + 16*n^3)/3
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 659, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type. */
    seq_t nn  = (seq_t)n;          /* n */
    seq_t nn2 = nn * nn;           /* n^2 */
    seq_t nn3 = nn2 * nn;          /* n^3 */

    /* Compute the numerator: 16*n^3 + 8*n - 21*n^2  */
    seq_t term = (seq_t)16 * nn3 + (seq_t)8 * nn - (seq_t)21 * nn2;

    /* Divide by 3; the result is always an integer. */
    return term / (seq_t)3;
}
