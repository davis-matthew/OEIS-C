/*
 * A127878  a(n) = n^4 + 4*n^3 + 12*n^2 + 24*n + 24.
 * https://oeis.org/A127878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 4*n^3 + 12*n^2 + 24*n + 24
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 254, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the sequence type */
    seq_t nn = (seq_t)n;

    /* Compute powers of n incrementally to minimize overflow risk */
    seq_t n2 = nn * nn;        /* n^2 */
    seq_t n3 = n2 * nn;        /* n^3 */
    seq_t n4 = n3 * nn;        /* n^4 */

    /* Combine terms according to the formula */
    seq_t result = n4 + 4 * n3 + 12 * n2 + 24 * nn + 24;

    return result;
}
