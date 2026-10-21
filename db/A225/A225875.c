/*
 * A225875  We write the 1 + 4*k numbers once and twice the others.
 * https://oeis.org/A225875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor(3*n/7)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t q = (N * (seq_t)3) / (seq_t)7;   /* floor(3*n/7) */
    return N - q;
}
