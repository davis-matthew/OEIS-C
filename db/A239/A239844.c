/*
 * A239844  Number of n X 2 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it or two plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A239844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 2*n^3 + 11*n^2 + 106*n - 144)/24 (n > 2), a(1) =
 *            3, a(2) = 8
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)8;

    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;

    seq_t numer = n4 + 2 * n3 + 11 * n2 + 106 * N - 144;
    return numer / 24;
}
