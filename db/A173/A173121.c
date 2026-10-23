/*
 * A173121  a(n) = sinh(2*arccosh(n))^2 = 4*n^2*(n^2 - 1).
 * https://oeis.org/A173121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2*(n^2-1)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 181, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn  = (seq_t)n;
    seq_t tn2 = tn * tn;
    return 4 * tn2 * (tn2 - 1);
}
