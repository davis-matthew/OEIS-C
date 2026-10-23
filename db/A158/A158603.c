/*
 * A158603  a(n) = 441*n^2 + 21.
 * https://oeis.org/A158603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 441*n^2 + 21
 * Range:     u8 n <= 0, u16 n <= 12, u32 n <= 3120, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    nn = nn * nn;        /* n^2 */
    nn = nn * 441u;      /* 441*n^2 */
    return nn + 21u;     /* +21 */
}
