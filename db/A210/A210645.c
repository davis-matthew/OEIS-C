/*
 * A210645  Area A of the triangles such that A, the sides and one of the
 *          altitudes are four consecutive integers of an arithmetic
 *          progression d.
 * https://oeis.org/A210645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 84*n^2
 * Range:     u8 n <= 1, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return (seq_t)84 * tn * tn;
}
