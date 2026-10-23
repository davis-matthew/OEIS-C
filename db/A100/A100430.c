/*
 * A100430  Bisection of A002417.
 * https://oeis.org/A100430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^4 +28*n^3 +34*n^2 +17*n+3)/3
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 198, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t numer = 8 * n4 + 28 * n3 + 34 * n2 + 17 * nn + (seq_t)3;
    return numer / (seq_t)3;
}
