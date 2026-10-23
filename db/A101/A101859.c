/*
 * A101859  a(n) = 11 + (23*n)/2 + n^2/2.
 * https://oeis.org/A101859
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^2 + 23*n + 22)/2
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < -1) return 0;
    seq_t tn = (seq_t)n;
    seq_t num = tn * tn + (seq_t)23 * tn + (seq_t)22;
    return num / (seq_t)2;
}
