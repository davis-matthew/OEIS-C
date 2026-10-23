/*
 * A017660  a(n) = (12*n + 11)^8.
 * https://oeis.org/A017660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 11)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 20, u128 n <= 5460,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 12 + 11;
    seq_t b2 = base * base;     /* base^2 */
    seq_t b4 = b2 * b2;         /* base^4 */
    return b4 * b4;             /* base^8 */
}
