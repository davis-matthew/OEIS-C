/*
 * A201043  Number of -n..n arrays of 4 elements with adjacent element
 *          differences also in -n..n.
 * https://oeis.org/A201043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (29*n^4 + 58*n^3 + 51*n^2 + 22*n + 4) / 4
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 110, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m  = (seq_t)n;
    seq_t n2 = m * m;
    seq_t n3 = n2 * m;
    seq_t n4 = n3 * m;
    seq_t sum = 29 * n4 + 58 * n3 + 51 * n2 + 22 * m + 4;
    return sum / 4;
}
