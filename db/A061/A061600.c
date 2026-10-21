/*
 * A061600  a(n) = n^3 - n + 1.
 * https://oeis.org/A061600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 - n + 1
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first. */
    seq_t nn = (seq_t)n;
    /* Compute n^3 - n + 1 using only integer operations. */
    return nn * nn * nn - nn + (seq_t)1;
}
