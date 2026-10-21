/*
 * A082109  Third row of number array A082105.
 * https://oeis.org/A082109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*n^2 + 8*n + 1
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the arithmetic type seq_t first. */
    seq_t nn = (seq_t)n;
    /* Compute the quadratic polynomial. */
    return 4*nn*nn + 8*nn + 1;
}
