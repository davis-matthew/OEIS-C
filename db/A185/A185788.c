/*
 * A185788  Sum of the first k-1 numbers in the k-th column of the natural
 *          number array A000027, by antidiagonals.
 * https://oeis.org/A185788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(7*n^2 - 11*n + 6)/6
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 956, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n1 = nn - 1;
    seq_t term = n1 * (7 * nn * nn - 11 * nn + 6) / 6;
    return term;
}
