/*
 * A060883  a(n) = n^6 + n^3 + 1.
 * https://oeis.org/A060883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 + n^3 + 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type once. */
    seq_t nn = (seq_t)n;

    /* Compute n^2, n^3 and n^6 using only seq_t arithmetic. */
    seq_t n2 = nn * nn;      /* n^2 */
    seq_t n3 = n2 * nn;      /* n^3 */
    seq_t n6 = n3 * n3;      /* n^6 */

    /* Return the final expression. The constant is cast to seq_t. */
    return n6 + n3 + (seq_t)1;
}
