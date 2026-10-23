/*
 * A152064  a(n) = 2*n^3 - 3*n^2 + 5.
 * https://oeis.org/A152064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3 - 3*n^2 + 5
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1290, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t sq   = nn * nn;          /* n^2 */
    seq_t cu   = sq * nn;          /* n^3 */
    seq_t res  = (seq_t)2 * cu + (seq_t)5 - (seq_t)3 * sq;
    return res;
}
