/*
 * A210369  Number of 2 X 2 matrices with all terms in {0,1,...,n} and even
 *          determinant.
 * https://oeis.org/A210369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+1)^2 - ceil(n/2)^2)^2 + ceil(n/2)^4
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 286, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t before any arithmetic */
    seq_t sn = (seq_t)n;

    /* ceil(n/2) = (n + 1) / 2 */
    seq_t half = (sn + 1) / 2;

    /* (n+1)^2 */
    seq_t n1sq = (sn + 1) * (sn + 1);

    /* ceil(n/2)^2 */
    seq_t halfsq = half * half;

    /* ((n+1)^2 - ceil(n/2)^2)^2 */
    seq_t diff = n1sq - halfsq;
    seq_t diffsq = diff * diff;

    /* ceil(n/2)^4 */
    seq_t half4 = halfsq * halfsq;

    /* final result */
    return diffsq + half4;
}
