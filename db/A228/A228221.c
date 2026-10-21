/*
 * A228221  Number of second differences of arrays of length 6 of numbers in
 *          0..n.
 * https://oeis.org/A228221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 35*n^4 + 14*n^3 - 17*n^2 + 30*n + 1
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 105, u64 n <= 177, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;
    seq_t n2 = nt * nt;
    seq_t n3 = n2 * nt;
    seq_t n4 = n3 * nt;
    seq_t res = 35 * n4 + 14 * n3 + 30 * nt + 1;
    res -= 17 * n2;
    return res;
}
