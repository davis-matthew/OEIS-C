/*
 * A069079  a(n) = (2*n+1)*(2*n+2)*(2*n+4)*(2*n+5).
 * https://oeis.org/A069079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+1)*(2*n+2)*(2*n+4)*(2*n+5)
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t f1 = nn * 2 + 1;
    seq_t f2 = nn * 2 + 2;
    seq_t f3 = nn * 2 + 4;
    seq_t f4 = nn * 2 + 5;
    return f1 * f2 * f3 * f4;
}
