/*
 * A086274  Antidiagonal sums of A086272 (and of A086273).
 * https://oeis.org/A086274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n^3 + 6*n^2 - n + 18)/24
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to the integer type used for the sequence values */
    seq_t N = n;
    seq_t N2 = N * N;        /* n^2 */
    seq_t N3 = N2 * N;       /* n^3 */
    /* compute numerator: n * (n^3 + 6*n^2 - n + 18) */
    seq_t numerator = N * (N3 + 6 * N2 - N + 18);
    /* denominator is 24, resulting in an integer value */
    return numerator / 24;
}
