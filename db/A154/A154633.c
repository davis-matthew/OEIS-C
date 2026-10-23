/*
 * A154633  a(n) = (4*n+1)*(4*n+3)*(4*n+5)*(4*n+7).
 * https://oeis.org/A154633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*n+1)*(4*n+3)*(4*n+5)*(4*n+7)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t a1 = 4*nn + (seq_t)1;
    seq_t a2 = 4*nn + (seq_t)3;
    seq_t a3 = 4*nn + (seq_t)5;
    seq_t a4 = 4*nn + (seq_t)7;
    seq_t res = a1 * a2;
    res *= a3;
    res *= a4;
    return res;
}
