/*
 * A201813  Number of arrays of 5 integers in -n..n with sum zero and equal
 *          numbers of elements greater than zero and less than zero.
 * https://oeis.org/A201813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*n^3 + 30*n + 1
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return ((20 * tn * tn) * tn) + (30 * tn) + (seq_t)1;
}
