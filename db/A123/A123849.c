/*
 * A123849  Number of ways to build a contiguous building with n LEGO blocks
 *          of size 5 X 5 on top of a fixed block of the same size so that the
 *          building is symmetric after a rotation by 90 degrees.
 * https://oeis.org/A123849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 for n<=3; a(n) = 10*n - 29 for n>=4
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    return nn * (seq_t)10 - (seq_t)29;
}
