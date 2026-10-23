/*
 * A047669  Row 5 of array in A047666.
 * https://oeis.org/A047669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^4 + 30*n^2 + 43)/15
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t ss = (seq_t)n;
    seq_t n2 = ss * ss;
    seq_t n4 = n2 * n2;
    seq_t term = (seq_t)2 * n4 + (seq_t)30 * n2 + (seq_t)43;
    return ss * term / (seq_t)15;
}
