/*
 * A133766  a(n) = (4*n+1)*(4*n+3)*(4*n+5).
 * https://oeis.org/A133766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+1)*(4*n+3)*(4*n+5)
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t a = 4 * N + 1;
    seq_t b = 4 * N + 3;
    seq_t c = 4 * N + 5;
    return a * b * c;
}
