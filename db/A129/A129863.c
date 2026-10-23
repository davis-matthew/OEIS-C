/*
 * A129863  Sums of three consecutive pentagonal numbers.
 * https://oeis.org/A129863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (9/2)*n^2 + (15/2)*n + 6 (i.e. a(n) = P(n)+P(n+1)+P(n+2)
 *            )
 * Range:     u8 n <= 4, u16 n <= 85, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* cast index to seq_t */
    seq_t val = 9 * nn * nn;        /* 9*n^2  */
    val += 15 * nn;                 /* +15*n   */
    val += 12;                      /* +12     */
    val /= 2;                       /* divide by 2, result is integer */
    return val;
}
