/*
 * A195859  a(n) = n^8-n.
 * https://oeis.org/A195859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 - n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 256,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;          /* n */
    seq_t x2 = x * x;             /* n^2 */
    seq_t x4 = x2 * x2;           /* n^4 */
    seq_t x8 = x4 * x4;           /* n^8 */
    return x8 - x;                /* n^8 - n */
}
