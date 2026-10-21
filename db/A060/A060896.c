/*
 * A060896  a(n) = n^12 - n^6 + 1.
 * https://oeis.org/A060896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^12 - n^6 + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n;
    seq_t x2  = x * x;          /* n^2 */
    seq_t x3  = x2 * x;         /* n^3 */
    seq_t x6  = x3 * x3;        /* n^6 */
    seq_t x12 = x6 * x6;        /* n^12 */
    return x12 - x6 + (seq_t)1;
}
