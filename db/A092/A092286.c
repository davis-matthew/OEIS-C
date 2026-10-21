/*
 * A092286  Fourth diagonal (m=3) of triangle A084938; a(n) = A084938(n+3,n) =
 *          (n^3 + 9*n^2 + 26*n)/6.
 * https://oeis.org/A092286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^3 + 9*n^2 + 26*n)/6
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t term = n3 + (seq_t)9 * n2 + (seq_t)26 * nn;
    return term / (seq_t)6;
}
