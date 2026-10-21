/*
 * A066107  a(0) = 0; for n > 0, a(2n+1) = (2n+1)*(2n+3); a(2n) = 2n + 2.
 * https://oeis.org/A066107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0; a(2n) = 2n + 2 for n > 0; a(2n+1) = (2n+1)*(2n+3)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if ((n & 1) != 0) { /* odd n */
        seq_t nn = (seq_t)n;
        return nn * (nn + (seq_t)2);
    } else { /* even n > 0 */
        seq_t nn = (seq_t)n;
        return (seq_t)2 * nn + (seq_t)2;
    }
}
