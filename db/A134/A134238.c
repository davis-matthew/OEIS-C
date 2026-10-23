/*
 * A134238  Row sums of triangle A134237.
 * https://oeis.org/A134238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^2 - 7*n + 4)/2
 * Range:     u8 n <= 10, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast idx_t to seq_t to keep arithmetic in the proper type. */
    seq_t nn = (seq_t)n;

    /* Compute (5*n^2 - 7*n + 4)/2. */
    seq_t val = ((seq_t)5 * nn * nn - (seq_t)7 * nn + (seq_t)4) / (seq_t)2;

    return val;
}
