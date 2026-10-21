/*
 * A213772  Principal diagonal of the convolution array A213771.
 * https://oeis.org/A213772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n*n - 3*n + 1)*n/2
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)4 * nn * nn; /* 4*n^2 */
    val -= (seq_t)3 * nn;           /* -3*n */
    val += 1;                       /* +1 */
    val *= nn;                      /* *n */
    val /= 2;                       /* /2 */
    return val;
}
