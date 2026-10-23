/*
 * A033372  a(n) = floor(52/n).
 * https://oeis.org/A033372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(52/n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >=1 by the OEIS offset, but guard just in case */
    if (n == 0) return 0;
    return (seq_t)52 / (seq_t)n;
}
