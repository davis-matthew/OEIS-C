/*
 * A082106  Main diagonal of number array A082105.
 * https://oeis.org/A082106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n^4 + 4*n^2 + 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t xn = (seq_t)n;        /* convert index to seq_t */
    return xn * xn * xn * xn + 4 * xn * xn + 1;
}
