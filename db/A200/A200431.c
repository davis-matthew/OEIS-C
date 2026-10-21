/*
 * A200431  Number of -n..n arrays x(0..3) of 4 elements with zero sum and no
 *          two or three adjacent elements summing to zero.
 * https://oeis.org/A200431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (16*n^3 - 18*n^2 + 2*n)/3 (This is the cubic polynomial
 *            that evaluates to an integer for all integer n.)
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num = 16 * n3 - 18 * n2 + 2 * nn;
    return num / 3;
}
