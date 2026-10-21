/*
 * A222408  Partial sums of A008531, or crystal ball sequence for {A_4}*
 *          lattice.
 * https://oeis.org/A222408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^4 + 10*n^3 + 15*n^2 + 10*n + 4)/4
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;          /* n^2 */
    seq_t n3 = n2 * n1;          /* n^3 */
    seq_t n4 = n3 * n1;          /* n^4 */
    seq_t num = 5 * n4 + 10 * n3 + 15 * n2 + 10 * n1 + 4;
    return num / 4;
}
