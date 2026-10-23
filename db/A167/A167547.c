/*
 * A167547  The fourth row of the ED1 array A167546.
 * https://oeis.org/A167547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*n^3 - 7*n^2 + 14*n - 8
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 850, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    return 7 * n3 - 7 * n2 + 14 * nn - 8;
}
