/*
 * A059605  a(n) = (1/3!)*(n^3 + 24*n^2 + 107*n + 90), compare A059604.
 * https://oeis.org/A059605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^3 + 24*n^2 + 107*n + 90)/6
 * Range:     u8 n <= 1, u16 n <= 32, u32 n <= 1629, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = nn * nn * nn + (seq_t)24 * nn * nn + (seq_t)107 * nn + (seq_t)90;
    return num / (seq_t)6;
}
