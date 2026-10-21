/*
 * A227970  Triangular arithmetic on half-squares: b(n)*(b(n) - 1)/2 where
 *          b(n) = floor(n^2/2).
 * https://oeis.org/A227970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial( floor(n^2/2), 2 ) = floor(n^2/2) *
 *            (floor(n^2/2)-1)/2
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute b(n) = floor(n^2/2) */
    seq_t b = ((seq_t)n * (seq_t)n) / 2;

    /* a(n) = binomial(b,2). Handle small b explicitly to avoid underflow. */
    if (b < 2)
        return (seq_t)0;

    return b * (b - 1) / 2;
}
