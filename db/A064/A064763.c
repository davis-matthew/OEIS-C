/*
 * A064763  a(n) = 28*n^2.
 * https://oeis.org/A064763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*n^2
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;   /* cast index to seq_t first */
    nn *= nn;             /* nn = n^2 */
    nn *= (seq_t)28;      /* multiply by 28 */
    return nn;
}
