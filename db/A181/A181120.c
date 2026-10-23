/*
 * A181120  Partial sums of round(n^2/12) (A069905).
 * https://oeis.org/A181120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 - 6*n + 9)/72)
 * Range:     u8 n <= 20, u16 n <= 132, u32 n <= 870, u64 n <= 870,
 *            u128 n <= 870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = (2*nn*nn*nn + 3*nn*nn - 6*nn + 9) / (seq_t)72;
    return term;
}
