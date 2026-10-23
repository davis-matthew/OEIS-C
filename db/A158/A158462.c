/*
 * A158462  a(n) = 36*n^2 - 6.
 * https://oeis.org/A158462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*n^2 - 6
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result = nn * nn;    /* n^2 */
    result = result * 36;      /* 36 * n^2 */
    result = result - 6;       /* subtract 6 */
    return result;
}
