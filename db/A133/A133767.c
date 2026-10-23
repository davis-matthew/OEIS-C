/*
 * A133767  a(n) = (4*n+3)*(4*n+5)*(4*n+7).
 * https://oeis.org/A133767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+3)*(4*n+5)*(4*n+7)
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = 4 * nn + 3;
    seq_t t2 = 4 * nn + 5;
    seq_t t3 = 4 * nn + 7;
    return t1 * t2 * t3;
}
