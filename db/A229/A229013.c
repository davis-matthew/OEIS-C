/*
 * A229013  Number of arrays of median of three adjacent elements of some
 *          length-5 0..n array, with no adjacent equal elements in the
 *          latter.
 * https://oeis.org/A229013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 + 3*n^2 - 3*n + 1
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    return sq * nn + 3 * sq - 3 * nn + 1;
}
