/*
 * A101377  a(n) = n^2*(n^3-n+2)/2.
 * https://oeis.org/A101377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^3-n+2)/2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 97, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the sequence type first */
    seq_t N = (seq_t)n;

    /* compute n^2 and n^3 */
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;

    /* A = n^3 - n + 2  (always even) */
    seq_t A = n3 - N + 2U;

    /* B = A / 2 */
    seq_t B = A / 2U;

    /* result = n^2 * B */
    return n2 * B;
}
