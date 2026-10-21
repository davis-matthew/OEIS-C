/*
 * A222170  a(n) = n^2 + 2*floor(n^2/3).
 * https://oeis.org/A222170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*n^2/3)
 * Range:     u8 n <= 12, u16 n <= 198, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to the sequence type before arithmetic */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t val = (5 * n2) / 3;    /* floor division by 3 */
    return val;
}
