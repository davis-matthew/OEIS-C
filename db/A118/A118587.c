/*
 * A118587  Expansion of (17-25*x-23*x^2+133*x^3)/(1-x)^4.
 * https://oeis.org/A118587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 17*n^3 - 62*n^2 + 71*n + 17
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    return (seq_t)17 * N3 - (seq_t)62 * N2 + (seq_t)71 * N + (seq_t)17;
}
