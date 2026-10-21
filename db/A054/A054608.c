/*
 * A054608  a(n) = Sum_{d|9} phi(d)*n^(9/d).
 * https://oeis.org/A054608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9 + 2*n^3 + 6*n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t nn  = (seq_t)n;

    /* Compute low powers of n */
    seq_t n2  = nn * nn;         /* n^2 */
    seq_t n3  = n2 * nn;         /* n^3 */
    seq_t n6  = n3 * n3;         /* n^6 */
    seq_t n9  = n6 * n3;         /* n^9  = n^6 * n^3 */

    /* Assemble the final value */
    return n9 + 2 * n3 + 6 * nn;
}
