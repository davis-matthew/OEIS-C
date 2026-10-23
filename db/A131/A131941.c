/*
 * A131941  Partial sums of ceiling(n^2/2) (A000982).
 * https://oeis.org/A131941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 + 3*n + 4)/12 for even n; * a(n) = (n+1)*(2*n^2
 *            + n + 3)/12 for odd n
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 2302, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1ULL) == 0) {
        seq_t num = nn * (2 * nn * nn + 3 * nn + 4);
        return num / 12;
    } else {
        seq_t num = (nn + 1) * (2 * nn * nn + nn + 3);
        return num / 12;
    }
}
