/*
 * A069130  Centered 17-gonal numbers: (17*n^2 - 17*n + 2)/2.
 * https://oeis.org/A069130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = (17*n^2 - 17*n + 2)/2 for n ≥ 1
 * Range:     u8 n <= 5, u16 n <= 88, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = nn * (nn - 1);          // n*(n-1)
    seq_t res = 1 + (17 * t) / 2;      // 1 + 17*n*(n-1)/2
    return res;
}
