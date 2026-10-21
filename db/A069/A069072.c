/*
 * A069072  a(n) = (2n+1)*(2n+2)*(2n+3).
 * https://oeis.org/A069072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2n+1)*(2n+2)*(2n+3)
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;           /* convert index to seq_t */
    seq_t t1 = x * 2 + 1;         /* 2n + 1 */
    seq_t t2 = x * 2 + 2;         /* 2n + 2 */
    seq_t t3 = x * 2 + 3;         /* 2n + 3 */
    return t1 * t2 * t3;          /* product of three consecutive integers */
}
