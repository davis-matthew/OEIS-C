/*
 * A201812  Number of arrays of 4 integers in -n..n with sum zero and equal
 *          numbers of elements greater than zero and less than zero.
 * https://oeis.org/A201812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n^3 + 14*n + 1
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t cubic = nn * nn * nn;
    return (seq_t)4 * cubic + (seq_t)14 * nn + 1;
}
