/*
 * A123111  1+n^2+n^3+n^5+n^7; 10101101 in base n.
 * https://oeis.org/A123111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + n^2 + n^3 + n^5 + n^7 (OEIS A123111)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p  = (seq_t)n;      /* n as seq_t */
    seq_t n2 = p * p;         /* n^2 */
    seq_t n3 = n2 * p;        /* n^3 */
    seq_t n5 = n3 * n2;       /* n^5 */
    seq_t n7 = n5 * n2;       /* n^7 */
    return (seq_t)1 + n2 + n3 + n5 + n7;
}
