/*
 * A111802  a(n) = n^2 - n - 1 for n>3; a(1)=1; a(2)=2; a(3)=3.
 * https://oeis.org/A111802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n^2 - n - 1 for n >= 4; a(1)=1; a(2)=2; a(3)=3
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return (seq_t)n;           /* a(1)=1, a(2)=2, a(3)=3 */
    seq_t nn = (seq_t)n;
    return nn * nn - nn - 1;      /* n^2 - n - 1 for n > 3 */
}
