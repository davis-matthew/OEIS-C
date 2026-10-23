/*
 * A157626  a(n) = 100*n^2 - 151*n + 57.
 * https://oeis.org/A157626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*n^2 - 151*n + 57
 * Range:     u8 n <= 2, u16 n <= 26, u32 n <= 6554, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type once */
    seq_t N = (seq_t)n;
    /* Compute the quadratic polynomial directly.
       The operations are performed in seq_t, so if the result fits
       into seq_t it is exact; otherwise modulo behaviour on overflow
       is acceptable as per the specification. */
    return (seq_t)100 * N * N - (seq_t)151 * N + (seq_t)57;
}
