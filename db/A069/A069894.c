/*
 * A069894  Centered square numbers: a(n) = 4*n^2 + 4*n + 2.
 * https://oeis.org/A069894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 + 4*n + 2
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* compute 4*n^2 + 4*n + 2 */
    seq_t temp = nn * nn;          /* n^2  */
    temp = temp * 4;                /* 4*n^2 */
    temp = temp + 4 * nn;           /* +4*n */
    temp = temp + (seq_t)2;         /* +2   */
    return temp;
}
