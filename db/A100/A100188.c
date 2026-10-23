/*
 * A100188  Polar structured meta-anti-diamond numbers, the n-th number from a
 *          polar structured n-gonal anti-diamond number sequence.
 * https://oeis.org/A100188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^4 - 2*n^2 + 6*n) / 6
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 335, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;        /* convert index to seq_t */
    seq_t n2  = nn * nn;        /* n^2 */
    seq_t n4  = n2 * n2;        /* n^4 */
    seq_t val = 2 * n4 - 2 * n2 + 6 * nn;
    return val / 6;
}
