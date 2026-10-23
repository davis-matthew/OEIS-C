/*
 * A109130  Magic constant of smallest order-n perfect magic cube.
 * https://oeis.org/A109130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + n)/2 for n = 1 and n >= 5 a(n) = 0 otherwise
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1 || n >= 5) {
        seq_t nn = (seq_t)n;
        seq_t n2 = nn * nn;        /* n^2 */
        seq_t n4 = n2 * n2;        /* n^4 */
        return (n4 + nn) / (seq_t)2;
    } else {
        return (seq_t)0;
    }
}
