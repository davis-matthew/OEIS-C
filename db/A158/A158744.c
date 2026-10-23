/*
 * A158744  a(n) = 74*n^2 - 1.
 * https://oeis.org/A158744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 74*n^2 - 1
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 7618, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result = nn * nn;   /* n^2 */
    result *= (seq_t)74;      /* 74*n^2 */
    result -= (seq_t)1;       /* subtract 1 */
    return result;
}
